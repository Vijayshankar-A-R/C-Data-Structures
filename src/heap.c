#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "heap.h"

int hp_init(hp_t *h, size_t elem_sz, int (*cmp)(const void *, const void *), void (*free_ele)(void *)) {
	if (!h || !cmp || elem_sz == 0) return 0;
	
	h->root = NULL;
	h->elem_sz = elem_sz;
	h->size = 0;
	h->cmp = cmp;
	h->free_ele = free_ele;
	return 1;
}

static void __free_node(hp_node *n, void (*free_ele)(void *)) {
	if (!n) return;
	if (free_ele) free_ele(n->val);
	free(n->val);
	free(n);
}

static void __rec_free(hp_node *n, void (*free_ele)(void *)) {
	if (!n) return;
	__rec_free(n->left, free_ele);
	__rec_free(n->right, free_ele);
	__free_node(n, free_ele);
}

void hp_free(hp_t *h) {
	if (!h) return;
	__rec_free(h->root, h->free_ele);
	h->root = NULL;
	h->elem_sz = 0;
	h->size = 0;
	h->cmp = NULL;
	h->free_ele = NULL;
}

static hp_node *__create_node(const void *elem, size_t elem_sz) {
	/* allocate node struct correctly */
	hp_node *n = (hp_node *)malloc(sizeof(hp_node));
	if (!n) return NULL;
	void *val = malloc(elem_sz);
	if (!val) {
		free(n);
		return NULL;
	}
	n->val = val;
	memcpy(n->val, elem, elem_sz);
	n->left = NULL;
	n->right = NULL;
	n->parent = NULL;
	return n;
}

static void __swap_val(hp_node *a, hp_node *b) {
	void *t = a->val;
	a->val = b->val;
	b->val = t;
}

static void __bubble_up(hp_node *n, int (*cmp)(const void *, const void *)) {
	while (n->parent && cmp(n->val, n->parent->val) > 0) {
		__swap_val(n, n->parent);
		n = n->parent;
	}
}

static void __bubble_down(hp_node *n, int(*cmp)(const void *, const void *)) {
	while (1) {
		hp_node *l = n;

		if (n->left && cmp(n->left->val, l->val) > 0)
			l = n->left;
		if (n->right && cmp(n->right->val, l->val) > 0)
			l = n->right;
		if (l == n)
			break;

		__swap_val(l, n);
		n = l;
	}
}

static hp_node *__get_node(const hp_node *n, size_t i, size_t d) {
	if (d == 0) return (hp_node *)n;

	size_t div = (size_t)1 << (d - 1); // 2 ^ (d - 1)

	if (i >= div) 
		return __get_node(n->right, i - div, d - 1); //  Right subtree
	else
		return __get_node(n->left, i, d - 1); // Left subtree
}


int hp_insert(hp_t *h, const void *elem) {
	if (!h || !elem) return 0;
	hp_node *n = __create_node(elem, h->elem_sz);
	if (!n) return 0;

	h->size++;

	if (!h->root) {
		h->root = n;
		return 1;
	}

	size_t k = h->size / 2;
	size_t d = floor(log2(k));
	size_t o = k - ((size_t)1 << d);

	hp_node *p = __get_node(h->root, o, d);

	n->parent = p;

	if (!p->left) p->left = n;
	else p->right = n;

	__bubble_up(n, h->cmp);
	return 1;
}

int hp_peek(const hp_t *h, void *out) {
	if (!h || !h->root || !out) return 0;
	memcpy(out, h->root->val, h->elem_sz);
	return 1;
}

int hp_popmax(hp_t *h, void *out) {
	if (!h || !h->root || !out) return 0;
	memcpy(out, h->root->val, h->elem_sz);

	if (h->size == 1) {
		__free_node(h->root, h->free_ele);
		h->root = NULL;
		h->size = 0;
		return 1;
	}

	size_t k = h->size;
	size_t d = floor(log2(k));
	size_t o = k - ((size_t)1 << d);

	hp_node *last = __get_node(h->root, o, d);
	__swap_val(h->root, last);
	hp_node *p = last->parent;

	if (p->left == last)
		p->left = NULL;
	else 
		p->right = NULL;
	
	__free_node(last, h->free_ele);
	h->size--;

	__bubble_down(h->root, h->cmp);
	return 1;
}

