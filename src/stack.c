#include "stack.h"


int stack_init(stack_t *s, size_t elem_sz, void (*free_ele)(void *)) {
	return ll_init(s, elem_sz, free_ele);
}

void stack_free(stack_t *s) {
	ll_free(s);
}

int stack_isempty(const stack_t *s) {
	return ll_isempty(s);
}

int stack_push(stack_t *s, const void *elem) {
	return ll_inserthead(s, elem);
}

int stack_pop(stack_t *s, void *out) {
	return ll_deletehead(s, out);
}

int stack_peek(const stack_t *s, void *out) {
	return ll_gethead(s, out);
}
