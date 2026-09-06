#include <stdlib.h>
#include <string.h>
#include "hash.h"

// djb2 hash
static size_t __hash(const void *key, size_t size) {
	const unsigned char *p = key;
	size_t h = 5381;

	for (size_t i = 0; i < size; i++)
		h = ((h << 5) + h) + p[i];
	return h;
}

int __create_entry(const void *key, const void *val, size_t key_sz, size_t val_sz, __entry_t *out, struct __ctx *ctx) {
	if (!key || !val || !out) return 0;

	out->key = malloc(key_sz);
	if (!out->key) return 0;
	memcpy(out->key, key, key_sz);

	out->val = malloc(val_sz);
	if (!out->val) {
		free(out->key);
		return 0;
	}
	memcpy(out->val, val, val_sz);

	out->ctx = ctx;

	return 1;
}

void __destroy_entry(void *p) {
	__entry_t *entry = (__entry_t *)p;

	if (!entry) return;
	if (entry->ctx->key_free) entry->ctx->key_free(entry->key);
	free(entry->key);
	if (entry->ctx->val_free) entry->ctx->val_free(entry->val);
	free(entry->val);
}

int ht_init(hash_t *h, size_t key_sz, size_t val_sz, size_t buckets, void (*key_free)(void *), void (*val_free)(void *)) {
	if (!h || buckets == 0 || key_sz == 0 || val_sz == 0) return 0;

	linklist_t *arr = (linklist_t *)calloc(buckets, sizeof(linklist_t));
	if (!arr) return 0;

	for (size_t i = 0; i < buckets; ++i) 
		if (!ll_init(&arr[i], sizeof(__entry_t), __destroy_entry)) {
			for (size_t j = 0; j < i; ++j) ll_free(&arr[i]);
			free(arr);
			return 0;
		}

	h->arr = arr;
	h->key_sz = key_sz;
	h->val_sz = val_sz;
	h->buckets = buckets;
	h->__ctx = (struct __ctx){key_free, val_free};
	return 1;
}

void ht_free(hash_t *h) {
	if (!h) return;
	for (size_t i = 0; i < h->buckets; ++i) ll_free(&h->arr[i]);
	free(h->arr);
	
	h->arr = NULL;
	h->key_sz = 0;
	h->val_sz = 0;
	h->buckets = 0;
	h->__ctx = (struct __ctx){NULL, NULL};
}

int ht_put(hash_t *h, const void *key, const void *val) {
	size_t i = __hash(key, h->key_sz) % h->buckets;
	__entry_t e;
	if (!__create_entry(key, val, h->key_sz, h->val_sz, &e, &h->__ctx)) return 0;
	return ll_inserthead(&h->arr[i], &e);
}

int ht_get(const hash_t *h, const void *key, void *out) {
	size_t i = __hash(key, h->key_sz) % h->buckets;
	linklist_t *l = h->arr + i;
	for (ll_node *cur = ll_next(l->head); cur; cur = ll_next(NULL)) {
		__entry_t *e = (__entry_t *)cur->data;
		if (memcmp(e->key, key, h->key_sz) == 0) {
			if (out) memcpy(out, e->val, h->val_sz);
			return 1;
		}
	}
	return 0;
}

int ht_rem(hash_t *h, const void *key) {
	size_t i = __hash(key, h->key_sz) % h->buckets;
        linklist_t *l = h->arr + i;
	int j = 0;
        for (ll_node *cur = ll_next(l->head); cur; cur = ll_next(NULL), j++) {
            __entry_t *e = (__entry_t *)cur->data;
			if (memcmp(e->key, key, h->key_sz) == 0) {
				__entry_t removed;
				return ll_delete(l, j, &removed);
			}
        }
        return 0;
}
