// bst.h

#ifndef BST_H
#define BST_H

#include <stddef.h>

typedef struct bst_node bst_node;
typedef struct bst_t bst_t;

struct bst_t {
	bst_node *root;
	size_t elem_sz;
	int (*cmp)(const void *, const void *);
	void (*free_ele)(void *);
};

struct bst_node {
	void *val;
	bst_node *left;
	bst_node *right;
	size_t __ht; // AVL implementation
};

int 	bst_init(bst_t *b, size_t elem_sz, int (*cmp)(const void *, const void *), void (*free_ele)(void *));
void	bst_free(bst_t *b);

int	bst_insert(bst_t *b, const void *elem);
int	bst_delete(bst_t *b, const void *elem);
int	bst_contains(const bst_t *b, const void *elem);

//int	bst_preorder(bst_t *b, void *out);
//int	bst_inorder(bst_t *b, void *out);
//int	bst_postorder(bst_t *b, void *out);

#endif
