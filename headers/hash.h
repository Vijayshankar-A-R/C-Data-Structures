// hash.h

#ifndef HASH_H
#define HASH_H

#include "linklist.h"

struct __ctx {
		void (*key_free)(void *);
		void (*val_free)(void *);
};

typedef struct {
	void 	*key;
	void 	*val;
	struct __ctx *ctx;
} __entry_t;

typedef struct {
	linklist_t *arr;	// hash_t is an array of link_list of __entry_t
	size_t 	key_sz;
	size_t	val_sz;
	size_t	buckets;
	struct __ctx __ctx;
} hash_t;

int	ht_init(hash_t *h, size_t key_sz, size_t val_sz, size_t buckets, void (*key_free)(void *), void (*val_free)(void *));
void	ht_free(hash_t *h);

int	ht_put(hash_t *h, const void *key, const void *val);
int	ht_get(const hash_t *h, const void *key, void *out);
int	ht_rem(hash_t *h, const void *key);

#endif
