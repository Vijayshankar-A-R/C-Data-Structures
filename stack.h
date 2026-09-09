// stack.h
#ifndef STACK_H
#define STACK_H

#ifdef STACK_IMPLEMENTATION
	#ifndef LL_IMPLEMENTATION
		#define LL_IMPLEMENTATION
	#endif // LL_IMPLEMENTATION
#endif // STACK_IMPLEMENTATION

#include "linklist.h"

typedef linklist_t stack_t;

int 	stack_init(stack_t *s, size_t elem_sz, void (*free_ele)(void *));
void 	stack_free(stack_t *s);

int	stack_isempty(const stack_t *s);

int	stack_push(stack_t *s, const void *elem);
int	stack_pop(stack_t *s, void *out);
int	stack_peek(const stack_t *s, void *out);

#ifdef STACK_IMPLEMENTATION

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

#endif // STACK_IMPLEMENTATION

#endif // STACK_H
