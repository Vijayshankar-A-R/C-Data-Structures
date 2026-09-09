// double_ll.h

#ifndef DOUBLE_LL_H
#define DOUBLE_LL_H

#include <stddef.h> // size_t

typedef struct dll_node dll_node;
typedef struct double_ll_t double_ll_t;

int 	dll_init(double_ll_t *l, size_t elem_sz, void (*free_ele)(void *));
void 	dll_free(double_ll_t *l);

int 	dll_isempty(const double_ll_t *l);
size_t 	dll_size(const double_ll_t *l);

dll_node	*dll_insert(double_ll_t *l, size_t i, const void *elem);
dll_node 	*dll_inserthead(double_ll_t *l, const void *elem);
dll_node	*dll_inserttail(double_ll_t *l, const void *elem);

int dll_delete(double_ll_t *l, size_t i, void *out_elem);
int dll_deletehead(double_ll_t *l, void *out_elem);
int dll_deletetail(double_ll_t *l, void *out_elem);
int dll_delete_node(double_ll_t *l, dll_node *n, void *out_elem);

int dll_getelem(const double_ll_t *l, size_t i, void *out_elem);
int dll_gethead(const double_ll_t *l, void *out_elem);
int dll_gettail(const double_ll_t *l, void *out_elem);

#ifdef DLL_IMPLEMENTATION

struct dll_node {
    void *data;
    struct dll_node *next;
    struct dll_node *prev;
};

struct double_ll_t {
    dll_node *head;
    dll_node *tail;
    size_t elem_sz;
    void (*__free_ele)(void *);
};

#include <stdlib.h>
#include <string.h>

static void __dll_free_node(dll_node *n, void (*free_ele)(void *)) {
    if (!n) return;
    if (n->data) {
        if (free_ele) free_ele(n->data);
        free(n->data);
    }
    free(n);
}

static dll_node *__dll_create_node(const void *elem, size_t elem_sz) {
    dll_node *n = (dll_node *)malloc(sizeof(dll_node));
    if (!n) return NULL;
    n->data = malloc(elem_sz);
    if (!n->data) {
        __dll_free_node(n, NULL);
        return NULL;
    }
    memcpy(n->data, elem, elem_sz);
    n->next = NULL;
    n->prev = NULL;
    return n;
}

int dll_init(double_ll_t *l, size_t elem_sz, void (*free_ele)(void *)) {
    if (!l || elem_sz == 0) return 0;

    l->head = NULL;
    l->tail = NULL;
    l->elem_sz = elem_sz;
    l->__free_ele = free_ele;
    return 1;
}

static void __dll_node_rec_delete(dll_node *node, void (*free_ele)(void *)) {
    if (!node) return;
    __dll_node_rec_delete(node->next, free_ele);
    __dll_free_node(node, free_ele);
}

void dll_free(double_ll_t *l) {
    if (!l) return;

    __dll_node_rec_delete(l->head, l->__free_ele);
    l->head = NULL;
    l->tail = NULL;
    l->elem_sz = 0;
    l->__free_ele = NULL;
}

int dll_isempty(const double_ll_t *l) {
    return !l || !l->head;
}


size_t dll_size(const double_ll_t *l) {
    size_t s = 0;
    for (const dll_node *cur = l->head; cur; cur = cur->next) s++;
    return s;
}

dll_node *dll_inserthead(double_ll_t *l, const void *elem) {
    if (!l) return NULL;

    dll_node *n = __dll_create_node(elem, l->elem_sz);
    if (!n) return NULL;

    n->next = l->head;
    n->prev = NULL;
    if (l->head) {
        l->head->prev = n;
    } else {
        l->tail = n;
    }
    l->head = n;
    return n;
}

dll_node *dll_inserttail(double_ll_t *l, const void *elem) {
    if (!l) return NULL;
    if (!l->head) return dll_inserthead(l, elem);

    dll_node *n = __dll_create_node(elem, l->elem_sz);
    if (!n) return NULL;

    l->tail->next = n;
    n->prev = l->tail;
    l->tail = n;
    return n;
}

dll_node *dll_insert(double_ll_t *l, size_t i, const void *elem) {
    if (!l) return NULL;
    if (i == 0) return dll_inserthead(l, elem);

    dll_node *cur = l->head;
    while (cur && i > 1) {
        cur = cur->next;
        i--;
    }
    if (!cur) return NULL;

    dll_node *n = __dll_create_node(elem, l->elem_sz);
    if (!n) return NULL;

    n->next = cur->next;
    n->prev = cur;
    if (cur->next) {
        cur->next->prev = n;
    } else {
        l->tail = n;
    }
    cur->next = n;
    return n;
}

int dll_delete_node(double_ll_t *l, dll_node *n, void *out_elem) {
    if (!l || !n) return 0;

    if (n == l->head) {
        l->head = n->next;
    }
    if (n == l->tail) {
        l->tail = n->prev;
    }
    if (n->prev) {
        n->prev->next = n->next;
    }
    if (n->next) {
        n->next->prev = n->prev;
    }

    if (out_elem) {
        memcpy(out_elem, n->data, l->elem_sz);
    }
    __dll_free_node(n, l->__free_ele);
    return 1;
}

int dll_deletehead(double_ll_t *l, void *out_elem) {
    if (dll_isempty(l) || !out_elem) return 0;
    memcpy(out_elem, l->head->data, l->elem_sz);
    return dll_delete_node(l, l->head, out_elem);
}

int dll_deletetail(double_ll_t *l, void *out_elem) {
    if (dll_isempty(l) || !out_elem) return 0;
    memcpy(out_elem, l->tail->data, l->elem_sz);
    return dll_delete_node(l, l->tail, out_elem);
}

int dll_delete(double_ll_t *l, size_t i, void *out_elem) {
    if (dll_isempty(l) || !out_elem) return 0;
    if (i == 0) return dll_deletehead(l, out_elem);

    dll_node *cur = l->head;
    while (cur && i > 0) {
        cur = cur->next;
        i--;
    }
    if (!cur) return 0;

    memcpy(out_elem, cur->data, l->elem_sz);
    return dll_delete_node(l, cur, out_elem);
}

int dll_gethead(const double_ll_t *l, void *out_elem) {
    if (dll_isempty(l) || !out_elem) return 0;
    memcpy(out_elem, l->head->data, l->elem_sz);
    return 1;
}

int dll_gettail(const double_ll_t *l, void *out_elem) {
    if (dll_isempty(l) || !out_elem) return 0;
    memcpy(out_elem, l->tail->data, l->elem_sz);
    return 1;
}

int dll_getelem(const double_ll_t *l, size_t i, void *out_elem) {
    if (dll_isempty(l) || !out_elem) return 0;
    if (i == 0) return dll_gethead(l, out_elem);

    dll_node *cur = l->head;
    while (cur && i > 0) {
        cur = cur->next;
        i--;
    }
    if (!cur) return 0;

    memcpy(out_elem, cur->data, l->elem_sz);
    return 1;
}


#endif // DLL_IMPLEMENTATION
#endif // DOUBLE_LL_H
