// queue.h

#ifndef QUEUE_H
#define QUEUE_H

#ifdef QUEUE_IMPLEMENTATION
	#ifndef DLL_IMPLEMENTATION
		#define DLL_IMPLEMENTATION
	#endif // DLL_IMPLEMENTATION
#endif // QUEUE_IMPLEMENTATION

#include "double_ll.h"

typedef double_ll_t queue_t;

int	q_init(queue_t *q, size_t elem_sz, void (*free_ele)(void *));
void	q_free(queue_t *q);

int	q_isempty(const queue_t *q);
size_t	q_size(const queue_t *q);

int	enqueue(queue_t *q, const void *elem);
int	dequeue(queue_t *q, void *out);

#ifdef QUEUE_IMPLEMENTATION

int q_init(queue_t *q, size_t elem_sz, void (*free_ele)(void *)) {
	return dll_init(q, elem_sz, free_ele);
}

void q_free(queue_t *q) {
        dll_free(q);
}

int q_isempty(const queue_t *q) {
        return dll_isempty(q);
}

size_t q_size(const queue_t *q) {
	return dll_size(q);
}

int enqueue(queue_t *q, const void *elem) {
        return dll_inserttail(q, elem) != NULL;
}


int dequeue(queue_t *q, void *out) {
	return dll_deletehead(q, out);
}

#endif // QUEUE_IMPLEMENTATION
#endif // QUEUE_H
