// heap.h

#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

typedef struct heap_node hp_node;
typedef struct heap_t hp_t;

struct heap_t {
	hp_node *root;
	size_t elem_sz;
	size_t size;
	int (*cmp)(const void *, const void *);
	void (*free_ele)(void *);
};

struct heap_node {
	void *val;
	hp_node *left;
	hp_node *right;
	hp_node *parent;
};

int	hp_init(hp_t *h, size_t elem_sz, int (*cmp)(const void *, const void *), void (*free_ele)(void *));
void	hp_free(hp_t *h);

int	hp_insert(hp_t *h, const void *elem);
int	hp_peek(const hp_t *h, void *out);
int	hp_popmax(hp_t *h, void *out);

#endif
