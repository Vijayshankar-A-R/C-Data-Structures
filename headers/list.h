// list.h

#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct {
	unsigned char *data;
	size_t cap;
	size_t size;
	size_t elem_sz;
	void (*__free_ele)(void *);
} list_t;

int	list_init(list_t *l, size_t initial_cap, size_t elem_sz, void (*free_ele)(void *));
void	list_free(list_t *l);

size_t	list_size(const list_t *l);

int	add(list_t *l, const void *elem);

int	set(list_t *l, size_t i, const void *elem);

int	get(const list_t *l, size_t i, void *out);


#endif
