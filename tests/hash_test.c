#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

static void check(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        exit(1);
    }
}

static void test_int_hash(void) {
    hash_t h;
    int keys[] = {1, 2, 3};
    int values[] = {100, 200, 300};
    int out = 0;

    check(ht_init(&h, sizeof(int), sizeof(int), 3, NULL, NULL) == 1, "ht_init for ints");

    check(ht_put(&h, &keys[0], &values[0]) == 1, "put key 1");
    check(ht_put(&h, &keys[1], &values[1]) == 1, "put key 2");
    check(ht_put(&h, &keys[2], &values[2]) == 1, "put key 3");

    check(ht_get(&h, &keys[0], &out) == 1 && out == 100, "get key 1");
    check(ht_get(&h, &keys[1], &out) == 1 && out == 200, "get key 2");
    check(ht_get(&h, &keys[2], &out) == 1 && out == 300, "get key 3");

    check(ht_rem(&h, &keys[1]) == 1, "remove key 2");
    check(ht_get(&h, &keys[1], &out) == 0, "get removed key 2 fails");
    check(ht_get(&h, &keys[0], &out) == 1 && out == 100, "get key 1 after remove");
    check(ht_get(&h, &keys[2], &out) == 1 && out == 300, "get key 3 after remove");

    ht_free(&h);
}

static void test_hash_collision(void) {
    hash_t h;
    int keys[] = {4, 8};
    int values[] = {400, 800};
    int out = 0;

    check(ht_init(&h, sizeof(int), sizeof(int), 1, NULL, NULL) == 1, "ht_init with single bucket");

    check(ht_put(&h, &keys[0], &values[0]) == 1, "put key 4");
    check(ht_put(&h, &keys[1], &values[1]) == 1, "put key 8");

    check(ht_get(&h, &keys[0], &out) == 1 && out == 400, "get key 4 in collision bucket");
    check(ht_get(&h, &keys[1], &out) == 1 && out == 800, "get key 8 in collision bucket");

    check(ht_rem(&h, &keys[0]) == 1, "remove key 4 from collision bucket");
    check(ht_get(&h, &keys[0], &out) == 0, "get removed key 4 fails");
    check(ht_get(&h, &keys[1], &out) == 1 && out == 800, "get key 8 still present after collision remove");

    ht_free(&h);
}

static void test_string_hash(void) {
    hash_t h;
    const char *keys[] = {"apple", "banana", "cherry"};
    const char *values[] = {"red", "yellow", "dark red"};
    char out[16];

    check(ht_init(&h, strlen(keys[0]) + 1, sizeof(out), 5, NULL, NULL) == 1, "ht_init for strings");

    check(ht_put(&h, keys[0], values[0]) == 1, "put apple");
    check(ht_put(&h, keys[1], values[1]) == 1, "put banana");
    check(ht_put(&h, keys[2], values[2]) == 1, "put cherry");

    check(ht_get(&h, keys[0], out) == 1 && strcmp(out, values[0]) == 0, "get apple");
    check(ht_get(&h, keys[1], out) == 1 && strcmp(out, values[1]) == 0, "get banana");
    check(ht_get(&h, keys[2], out) == 1 && strcmp(out, values[2]) == 0, "get cherry");

    check(ht_rem(&h, keys[0]) == 1, "remove apple");
    check(ht_get(&h, keys[0], out) == 0, "get removed apple fails");

    ht_free(&h);
}

int main(void) {
    test_int_hash();
    test_hash_collision();
    test_string_hash();
    puts("All hash tests passed.");
    return 0;
}
