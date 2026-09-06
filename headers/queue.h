// queue.h

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

#ifndef DSA_LL_NODE_DEFINED
#define DSA_LL_NODE_DEFINED
struct ll_node {
	void *data;
	struct ll_node *next;
};

typedef struct ll_node ll_node;
#endif

typedef struct {
	ll_node *front;
	ll_node *rear;
	size_t elem_sz;
	void (*__free_ele)(void *);
} queue_t;

int	q_init(queue_t *q, size_t elem_sz, void (*free_ele)(void *));
void	q_free(queue_t *q);

int	q_isempty(const queue_t *q);
size_t	q_size(const queue_t *q);

int	enqueue(queue_t *q, const void *elem);
int	dequeue(queue_t *q, void *out);

#endif
