// stack.h
#ifndef LL_STACK_H
#define LL_STACK_H

#include "linklist.h"

typedef linklist_t stack_t;

int 	stack_init(stack_t *s, size_t elem_sz, void (*free_ele)(void *));
void 	stack_free(stack_t *s);

int	stack_isempty(const stack_t *s);

int	stack_push(stack_t *s, const void *elem);
int	stack_pop(stack_t *s, void *out);
int	stack_peek(const stack_t *s, void *out);

#endif
