#include <stdlib.h>
#include <string.h>
#include "bst.h"

int bst_init(bst_t *b, size_t elem_sz, int (*cmp)(const void *, const void *), void (*free_ele)(void *)) {
	if (elem_sz == 0 || !cmp || !b) return 0;
	b->root = NULL;
	b->elem_sz = elem_sz;
	b->cmp = cmp;
	b->free_ele = free_ele;
	return 1;
}

static void __free_bst_node(bst_node *n, void (*free_ele)(void *)) {
	if (!n) return;
	if (free_ele) free_ele(n->val);
	free(n->val);
	free(n);
}

static void __rec_free(bst_node *n, void (*free_ele)(void *)) {
	if (!n) return;
	__rec_free(n->left, free_ele);
	__rec_free(n->right, free_ele);
	__free_bst_node(n, free_ele);
}

void bst_free(bst_t *b) {
	if (!b) return;
	__rec_free(b->root, b->free_ele);
	b->root = NULL;
	b->elem_sz = 0;
	b->cmp = NULL;
	b->free_ele = NULL;
}

static bst_node *__create_node(const void *elem, size_t elem_sz) {
	bst_node *n = (bst_node *)malloc(sizeof(bst_node));
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
	n->__ht = 1;
	return n;
}

static size_t __get_ht(const bst_node *n) {
	if (!n) return 0;
	return n->__ht;
}

static void __update_ht(bst_node *n) {
	if (!n) return;
	size_t a, b;
	a = __get_ht(n->left);
	b = __get_ht(n->right);
	n->__ht = (a > b ? a : b) + 1;
}

static int __bf(const bst_node *n) {
	if (!n) return 0;
	return __get_ht(n->left) - __get_ht(n->right);
}

static bst_node *__rot_left(bst_node *n) {
	/* left rotation: pivot is right child */
	bst_node *x, *s;

	x = n->right;
	if (!x) return n;
	s = x->left;

	x->left = n;
	n->right = s;

	__update_ht(n);
	__update_ht(x);

	return x;
}

static bst_node *__rot_right(bst_node *n) {
	/* right rotation: pivot is left child */
	bst_node *x, *s;

	x = n->left;
	if (!x) return n;
	s = x->right;

	x->right = n;
	n->left = s;

	__update_ht(n);
	__update_ht(x);

	return x;
}

static bst_node *__rebal(bst_node *n) {
	__update_ht(n);

	int bf = __bf(n);

	// left heavy
	if (bf > 1) { 
		if (__bf(n->left) < 0)
			n->left = __rot_left(n->left); // LR
		return __rot_right(n); // LL
	}
	
	// right heavy
	if (bf < -1) {
		if (__bf(n->right) > 0)
			n->right = __rot_right(n->right); // RL
		return __rot_left(n); // RR
	}

	return n;
}

static bst_node *__rec_insert(bst_node *n, const void *elem, size_t elem_sz, int (*cmp)(const void *, const void *)) {
	if (!n) 
		return __create_node(elem, elem_sz);

	int k = cmp(elem, n->val);
	if (k == 0)
		return n;
	if (k > 0)
		n->right = __rec_insert(n->right, elem, elem_sz, cmp);
	if (k < 0)
		n->left = __rec_insert(n->left, elem, elem_sz, cmp);
	return __rebal(n);
}

int bst_insert(bst_t *b, const void *elem) {
	if (!b) return 0;

	b->root = __rec_insert(b->root, elem, b->elem_sz, b->cmp);
	return 1; // Doesnt guarantee insert success
}

static int __rec_contains(const bst_node *n, const void *elem, int (*cmp)(const void *, const void *)) {
	if (!n) return 0;
	int k = cmp(elem, n->val);
	if (k == 0)
		return 1;
	if (k > 0)
		return __rec_contains(n->right, elem, cmp);
	if (k < 0)
		return __rec_contains(n->left, elem, cmp);
	__builtin_unreachable();
}

int bst_contains(const bst_t *b, const void *elem) {
	if (!b) return 0;
	return __rec_contains(b->root, elem, b->cmp);
}

static bst_node *__rec_delete(bst_node *n, const void *elem, size_t elem_sz, int (*cmp)(const void *, const void *), void (*free_ele)(void *)) {
	if (!n) return NULL;

	int k = cmp(elem, n->val);

	if (k > 0)
		n->right = __rec_delete(n->right, elem, elem_sz, cmp, free_ele);
	else if (k < 0)
		n->left = __rec_delete(n->left, elem, elem_sz, cmp, free_ele);
	else {
		if (!n->right || !n->left) {
			bst_node *c;
			if (!n->left && !n->right)
				c = NULL;
			else
				c = !n->left ? n->right : n->left;
			__free_bst_node(n, free_ele);
			return c;
		}
		bst_node *suc = n->right;
		while (suc->left) suc = suc->left;

		if (free_ele) free_ele(n->val);	
		memcpy(n->val, suc->val, elem_sz);
		n->right = __rec_delete(n->right, suc->val, elem_sz, cmp, free_ele);
	}

	return __rebal(n);
}

int bst_delete(bst_t *b, const void *elem) {
	if (!b) return 0;
	
	b->root = __rec_delete(b->root, elem, b->elem_sz, b->cmp, b->free_ele);
	return 1; // Doesnt guarantee success
}
