// stack.h
#ifndef STACK_H
#define STACK_H

#include <stddef.h> // size_t

typedef struct stack_t stack_t;

struct stack_t {
	unsigned char *data;
    	size_t 	top;
    	size_t cap;
    	size_t elem_sz;
	void (*__free_ele)(void *);
};

int	stack_init(stack_t *s, size_t initial_capacity, size_t elem_sz, void (*free_ele)(void *));
void 	stack_free(stack_t *s);

int  	stack_isempty(const stack_t *s);

int  	stack_push(stack_t *s, const void *elem); // copies elem bytes into stack
int  	stack_pop(stack_t *s, void *out_elem);    // copies top bytes out into out_elem
int  	stack_peek(const stack_t *s, void *out_elem);

#endif

