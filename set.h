// set.h

#ifndef SET_H
#define SET_H

#include <stddef.h> // size_t

typedef struct hash_t set_t;

int set_init(set_t *s, size_t elem_sz, void (*free_ele)(void *));
// tip: use set_hash set_cmp for more complex datatypes

int set_insert(set_t *s, const void *elem);
int set_contains(set_t *s, const void *elem);
int set_remove(set_t *s, const void *elem);

void set_free(set_t *s);

#ifdef SET_IMPLEMENTATION

#ifndef HASH_IMPLEMENTATION
#define HASH_IMPLEMENTATION
#endif // HASH_IMPLEMENTATION
#include "hash.h"

const char val = 'y';

int set_init(set_t *s, size_t elem_sz, void (*free_ele)(void *)) {
    return ht_init(s, elem_sz, sizeof(char), 30, free_ele, NULL);
}

int set_insert(set_t *s, const void *elem) {
    return ht_put(s, elem, &val);
}

int set_contains(set_t *s, const void *elem) {
    return ht_get(s, elem, NULL);
}

int set_remove(set_t *s, const void *elem) {
    return ht_rem(s, elem);
}

void set_free(set_t *s) {
    ht_free(s);
}

#endif // SET_IMPLEMENTATION
#endif // SET_H
