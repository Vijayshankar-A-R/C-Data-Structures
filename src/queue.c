#include <stdlib.h>
#include <string.h>
#include "queue.h"

static void __free_node(ll_node *n, void (*free_ele)(void *)) {
        if (!n) return;
        if (n->data) {
                if (free_ele) free_ele(n->data);
                free(n->data);
        }
        free(n);
}

static ll_node *__create_node(const void *elem, size_t elem_sz) {
        ll_node *n = (ll_node *)malloc(sizeof(ll_node));
        if (!n) return NULL;
        n->data = malloc(elem_sz);
        if (!n->data) {
                __free_node(n, NULL);
                free(n);
                return NULL;
        }
        memcpy(n->data, elem, elem_sz);
        n->next = NULL;
        return n;
}

int q_init(queue_t *q, size_t elem_sz, void (*free_ele)(void *)) {
	if (!q || elem_sz == 0) return 0;

	q->front = NULL;
	q->rear = NULL;
	q->elem_sz = elem_sz;
	q->__free_ele = free_ele;
	return 1;
}

// if node is not NULL, returns the current node
// if node is NULL, returns the next node from the last call
static ll_node *__ll_next(const ll_node *node) {
        static const ll_node *last;

        last = (!node) ? last : node;
        if (!last) return NULL;

        ll_node *ret = (ll_node *)last;
        last = last->next;
        return ret;
}

static void __node_rec_delete(ll_node *node, void (*free_ele)(void *)) {
        if (!node) return;
        __node_rec_delete(__ll_next(NULL), free_ele);
        __free_node(node, free_ele);
}

void q_free(queue_t *q) {
        if (!q) return;

        __node_rec_delete(__ll_next(q->front), q->__free_ele);

        q->front = NULL;
	q->rear = NULL;
        return;
}

int q_isempty(const queue_t *q) {
        return !q || q->front == NULL;
}

size_t q_size(const queue_t *q) {
        size_t s = 0;
        for (const ll_node *cur = __ll_next(q->front); cur; cur = __ll_next(NULL)) s++;
        return s;
}

int enqueue(queue_t *q, const void *elem) {
        if (!q) return 0;

        ll_node *n = __create_node(elem, q->elem_sz);
        if (!n) return 0;

        if (!q->rear) {
		n->next = NULL;
		q->rear = n;
		q->front = n;
		return 1;
	}

	q->rear->next = n;
	q->rear = n;
        return 1;
}


int dequeue(queue_t *q, void *out) {
	if (q_isempty(q) || !out) return 0;

	ll_node *tmp = q->front;
	q->front = tmp->next;
        if (!q->front) q->rear = NULL;
	memcpy(out, tmp->data, q->elem_sz);
	__free_node(tmp, q->__free_ele);
	return 1;
}
