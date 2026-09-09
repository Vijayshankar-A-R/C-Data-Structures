// list.h

#ifndef LIST_H
#define LIST_H

#include <stddef.h> // size_t

typedef struct list_t list_t;

int	list_init(list_t *l, size_t initial_cap, size_t elem_sz, void (*free_ele)(void *));
void	list_free(list_t *l);

size_t	list_size(const list_t *l);

int	add(list_t *l, const void *elem);

int	set(list_t *l, size_t i, const void *elem);

int	get(const list_t *l, size_t i, void *out);

#ifdef LIST_IMPLEMENTATION

struct list_t{
	unsigned char *data;
	size_t cap;
	size_t size;
	size_t elem_sz;
	void (*__free_ele)(void *);
};

#include <stdlib.h>
#include <string.h>

int list_init(list_t *l, size_t initial_cap, size_t elem_sz, void (*free_ele)(void *)) {
	if (!l || initial_cap == 0 || elem_sz == 0) return 0;

	l->data = calloc(initial_cap, elem_sz);
	if (!l->data) return 0;

	l->cap = initial_cap;
	l->size = 0;
	l->elem_sz = elem_sz;
	l->__free_ele = free_ele;
	return 1;
}

void list_free(list_t *l) {
	if (!l) return;


	if (l->data) {
		if (l->__free_ele)
			for (size_t i = 0; i < l->size; i++) l->__free_ele(&l->data[i * l->elem_sz]);
		free(l->data);
	}

	l->data = NULL;
	l->cap = 0;
	l->size = 0;
	l->elem_sz = 0;
	l->__free_ele = NULL;
}

size_t list_size(const list_t *l) {
	if (!l) return 0;
	return l->size;
}

static int __grow_list(list_t *l) {
	size_t new_cap = 2 * l->cap;

	void *new_data = reallocarray(l->data, new_cap, l->elem_sz);
	if (!new_data) return 0;

	l->data = new_data;
	l->cap = new_cap;
	return 1;
}

int add(list_t *l, const void *elem) {
	if (!l || !elem) return 0;
	if (l->size == l->cap && !__grow_list(l)) return 0;

	memcpy(&l->data[l->size++ * l->elem_sz], elem, l->elem_sz);
	return 1;
}

int set(list_t *l, size_t i, const void *elem) {
	if (!l || !elem) return 0;
	if (i > l->size) return 0;
	if (i == l->size) return add(l, elem);

	if (l->__free_ele) l->__free_ele(&l->data[i * l->elem_sz]);
	memcpy(&l->data[i * l->elem_sz], elem, l->elem_sz);
	return 1;
}

int get(const list_t *l, size_t i, void *out) {
	if (!l || !out) return 0;
	if (i >= l->size) return 0;

	memcpy(out, &l->data[i * l->elem_sz], l->elem_sz);
	return 1;
}

#endif // LIST_IMPLEMENTATION
#endif // LIST_H
