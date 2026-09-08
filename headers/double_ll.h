// double_ll.h

#ifndef DOUBLE_LL_H
#define DOUBLE_LL_H

#include <stddef.h> // size_t

struct dll_node {
    void *data;
    struct dll_node *next;
    struct dll_node *prev;
};

typedef struct dll_node dll_node;

typedef struct {
    dll_node *head;
    dll_node *tail;
    size_t elem_sz;
    void (*__free_ele)(void *);
} double_ll_t;

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

#endif
