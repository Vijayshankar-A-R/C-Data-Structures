#include <stdlib.h>
#include <string.h>
#include "linklist.h"

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
		return NULL;
	}
	memcpy(n->data, elem, elem_sz);
	n->next = NULL;
	return n;
}

int ll_init(linklist_t *l, size_t elem_sz, void (*free_ele)(void *)) {
	if (!l || elem_sz == 0) return 0;

	l->head = NULL;
	l->elem_sz = elem_sz;
	l->__free_ele = free_ele;
	return 1;
}

// if node is not NULL, returns the current node
// if node is NULL, returns the next node from the last call
ll_node *ll_next(const ll_node *node) {
	static const ll_node *last;

	last = (!node) ? last : node;
	if (!last) return NULL;

	ll_node *ret = (ll_node *)last;
	last = last->next;
	return ret;
}

static void __node_rec_delete(ll_node *node, void (*free_ele)(void *)) {
	if (!node) return;
	__node_rec_delete(ll_next(NULL), free_ele);
	__free_node(node, free_ele);
}

void ll_free(linklist_t *l) {
	if (!l) return;

	__node_rec_delete(ll_next(l->head), l->__free_ele);

	l->head = NULL;
	return;
}

int ll_isempty(const linklist_t *l) {
	return !l || !l->head;
}

size_t ll_size(const linklist_t *l) {
	size_t s = 0;
	for (const ll_node *cur = ll_next(l->head); cur; cur = ll_next(NULL)) s++;
	return s;
}

int ll_inserthead(linklist_t *l, const void *elem) {
	if (!l) return 0;

	ll_node *n = __create_node(elem, l->elem_sz);
	if (!n) return 0;

	n->next = l->head;
	l->head = n;
	return 1;
}

int ll_inserttail(linklist_t *l, const void *elem) {
	if (!l) return 0;

	if (!l->head) return ll_inserthead(l, elem);

	ll_node *n = __create_node(elem, l->elem_sz);
	if (!n) return 0;

	ll_node *prev;
	for (prev = ll_next(l->head); prev->next; prev = ll_next(NULL));

	prev->next = n;
	return 1;
}

int ll_insert(linklist_t *l, size_t i, const void *elem) {
	if (!l) return 0;

	if (i == 0) return ll_inserthead(l, elem);

	ll_node *prev;
	for (prev = ll_next(l->head); --i; prev = ll_next(NULL));
	if (!prev) return 0;

	ll_node *n = __create_node(elem, l->elem_sz);
	if (!n) return 0;

	n->next = prev->next;
	prev->next = n;
	return 1;
}

int ll_deletehead(linklist_t *l, void *out_elem) {
	if (ll_isempty(l) || !out_elem) return 0;

	ll_node *tmp = l->head;
	l->head = tmp->next;
	memcpy(out_elem, tmp->data, l->elem_sz);
	__free_node(tmp, l->__free_ele);
	return 1;
}

int ll_deletetail(linklist_t *l, void *out_elem) {
	if (ll_isempty(l) || !out_elem) return 0;

	if (l->head->next == NULL) return ll_deletehead(l, out_elem);

	ll_node *prev, *tmp;
	for (prev = ll_next(l->head); prev->next->next; prev = ll_next(NULL));

	tmp = prev->next;
	prev->next = NULL;
	memcpy(out_elem, tmp->data, l->elem_sz);
	__free_node(tmp, l->__free_ele);
	return 1;
}

int ll_delete(linklist_t *l, size_t i, void *out_elem) {
	if (ll_isempty(l) || !out_elem) return 0;

	if (i == 0) return ll_deletehead(l, out_elem);

	ll_node *prev, *tmp;
	for (prev = ll_next(l->head); --i; prev = ll_next(NULL));
	if (!prev) return 0;
	
	tmp = prev->next;
	if (!tmp) return 0;
	prev->next = tmp->next;
	memcpy(out_elem, tmp->data, l->elem_sz);
	__free_node(tmp, l ->__free_ele);
	return 1;
}

int ll_gethead(const linklist_t *l, void *out_elem) {
	if (ll_isempty(l) || !out_elem) return 0;
	memcpy(out_elem, l->head->data, l->elem_sz);
	return 1;
}

int ll_gettail(const linklist_t *l, void *out_elem) {	
	if (ll_isempty(l) || !out_elem) return 0;
	const ll_node *last;
	for (last = ll_next(l->head); last->next; last = ll_next(NULL));
	memcpy(out_elem, last->data, l->elem_sz);
	return 1;
}

int ll_getelem(const linklist_t *l, size_t i, void *out_elem) {
	if (ll_isempty(l) || !out_elem) return 0;

	if (i == 0) return ll_gethead(l, out_elem);

	const ll_node *n;
	for (n = ll_next(l->head); i--; n = ll_next(NULL));
	if (!n) return 0;

	memcpy(out_elem, n->data, l->elem_sz);
	return 1;
}
