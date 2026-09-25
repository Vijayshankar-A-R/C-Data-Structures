#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASH_IMPLEMENTATION
#include "hash.h"

static void check(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        exit(1);
    }
}

static int custom_hash_calls;
static int custom_cmp_calls;

static size_t constant_hash(const void *key, size_t size) {
    (void)key;
    (void)size;
    custom_hash_calls++;
    return 0;
}

static int integer_cmp(const void *left, const void *right, size_t size) {
    (void)size;
    custom_cmp_calls++;
    return *(const int *)left != *(const int *)right;
}

static void test_int_hash(void) {
    hash_t h;
    int keys[] = {1, 2, 3};
    int values[] = {100, 200, 300};
    int out = 0;

        check(ht_init(&h, sizeof(int), sizeof(int), 3, NULL, NULL) == 1,
                    "ht_init for ints");

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

        check(ht_init(&h, sizeof(int), sizeof(int), 1, NULL, NULL) == 1,
                    "ht_init with single bucket");

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
    char keys[3][7];
    char values[3][9];
    char out[16];

    strcpy(keys[0], "apple");
    strcpy(keys[1], "banana");
    strcpy(keys[2], "cherry");
    strcpy(values[0], "red");
    strcpy(values[1], "yellow");
    strcpy(values[2], "dark red");

    check(ht_init(&h, sizeof(keys[0]), sizeof(values[0]), 5, NULL, NULL) == 1,
          "ht_init for strings");

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

static void test_custom_hash(void) {
    hash_t h;
    int keys[] = {10, 20, 30};
    int values[] = {1000, 2000, 3000};
    int out = 0;

    custom_hash_calls = 0;
        check(ht_init(&h, sizeof(int), sizeof(int), 4, NULL, NULL) == 1,
            "ht_init for custom hash");
        set_hash(&h, constant_hash);

    check(ht_put(&h, &keys[0], &values[0]) == 1, "custom hash put key 10");
    check(ht_put(&h, &keys[1], &values[1]) == 1, "custom hash put key 20");
    check(ht_put(&h, &keys[2], &values[2]) == 1, "custom hash put key 30");

    check(ht_get(&h, &keys[0], &out) == 1 && out == 1000,
          "custom hash get key 10");
    check(ht_get(&h, &keys[1], &out) == 1 && out == 2000,
          "custom hash get key 20");
    check(ht_get(&h, &keys[2], &out) == 1 && out == 3000,
          "custom hash get key 30");

    check(ht_rem(&h, &keys[1]) == 1, "custom hash remove key 20");
    check(ht_get(&h, &keys[1], &out) == 0,
          "custom hash get removed key 20 fails");
    check(ht_get(&h, &keys[0], &out) == 1 && out == 1000,
          "custom hash collision key 10 remains");
    check(ht_get(&h, &keys[2], &out) == 1 && out == 3000,
          "custom hash collision key 30 remains");
    check(custom_hash_calls > 0, "custom hash callback is used");

    ht_free(&h);
}

    static void test_custom_cmp(void) {
        hash_t h;
        int keys[] = {10, 20};
        int values[] = {1000, 2000};
        int lookup = 20;
        int out = 0;

        custom_cmp_calls = 0;
        check(ht_init(&h, sizeof(int), sizeof(int), 3, NULL, NULL) == 1,
            "ht_init for custom comparator");
        set_cmp(&h, integer_cmp);

        check(ht_put(&h, &keys[0], &values[0]) == 1, "custom cmp put key 10");
        check(ht_put(&h, &keys[1], &values[1]) == 1, "custom cmp put key 20");
        check(ht_get(&h, &lookup, &out) == 1 && out == 2000,
            "custom cmp get key 20");
        check(ht_rem(&h, &lookup) == 1, "custom cmp remove key 20");
        check(ht_get(&h, &lookup, &out) == 0,
            "custom cmp get removed key 20 fails");
        check(custom_cmp_calls > 0, "custom comparator is used");

        ht_free(&h);
    }

int main(void) {
    test_int_hash();
    test_hash_collision();
    test_string_hash();
    test_custom_hash();
    test_custom_cmp();
    puts("All hash tests passed.");
    return 0;
}
