// queue.h

#ifndef QUEUE_H
#define QUEUE_H

#include "double_ll.h"

typedef double_ll_t queue_t;

int	q_init(queue_t *q, size_t elem_sz, void (*free_ele)(void *));
void	q_free(queue_t *q);

int	q_isempty(const queue_t *q);
size_t	q_size(const queue_t *q);

int	enqueue(queue_t *q, const void *elem);
int	dequeue(queue_t *q, void *out);

#endif
