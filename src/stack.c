#include <stdlib.h>
#include <string.h>
#include "stack.h"

int stack_init(stack_t *s, size_t elem_sz, size_t cap, void (*free_ele)(void *)) {
	if (!s) return 0;
	if (cap == 0) cap = 1;

	s->data = calloc(cap, elem_sz);
	if (!s->data) return 0;

	s->top = 0;
	s->cap = cap;
	s->elem_sz = elem_sz;
	s->__free_ele = free_ele;
	return 1;
}

void stack_free(stack_t *s) {
	if (!s) return;
	if (s->__free_ele) for (size_t i = 0; i < s->top; ++i) s->__free_ele(&s->data[i * s->elem_sz]);
	free(s->data);

	s->data = NULL;
	s->top = 0;
	s->cap = 0;
	s->elem_sz = 0;
	s->__free_ele = NULL;
}

int stack_isempty(const stack_t *s) {
        return !s || s->top == 0;
}

static int __stack_grow(stack_t *s) {
	size_t new_cap = (s->cap == 0) ? 4 : 2 * s->cap;

	void *new_data = reallocarray(s->data, new_cap, s->elem_sz);
	if (!new_data) return 0;

	s->data = new_data;
	s->cap = new_cap;
	return 1;
}

int stack_push(stack_t *s, const void *elem) {
	if (!s || !elem) return 0;

	if (s->top == s->cap && !__stack_grow(s)) return 0;

	memcpy(s->data + s->top * s->elem_sz, elem, s->elem_sz);
	s->top++;
	return 1;
}

// copy popped element to user given memory
int stack_pop(stack_t *s, void *buff) {
	if (stack_isempty(s) || !buff) return 0;

	s->top--;
	memcpy(buff, s->data + s->top * s->elem_sz, s->elem_sz);
	if(s->__free_ele) s->__free_ele(&s->data[s->top * s->elem_sz]);
	return 1;
}

//copy last element to user given memory
int stack_peek(const stack_t *s, void *buff) {
	if (stack_isempty(s) || !buff) return 0;

	memcpy(buff, &s->data[(s->top - 1) * s->elem_sz], s->elem_sz);
	return 1;
}
