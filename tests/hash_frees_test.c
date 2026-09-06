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

static int freed_keys = 0;
static int freed_vals = 0;

static void key_free(void *p) {
    (void)p;
    freed_keys++;
}

static void val_free(void *p) {
    (void)p;
    freed_vals++;
}

static void test_key_val_free(void) {
    hash_t h;
    const char *keys[] = {"one", "two"};
    const char *values[] = {"uno", "dos"};
    char out[16];

    freed_keys = 0;
    freed_vals = 0;

    check(ht_init(&h, 16, 16, 3, key_free, val_free) == 1, "ht_init with key_free and val_free");

    check(ht_put(&h, keys[0], values[0]) == 1, "put one");
    check(ht_put(&h, keys[1], values[1]) == 1, "put two");

    check(ht_rem(&h, keys[0]) == 1, "remove one");
    check(ht_get(&h, keys[0], out) == 0, "get removed one fails");
    check(ht_get(&h, keys[1], out) == 1 && strcmp(out, values[1]) == 0, "get two after remove");

    ht_free(&h);

    check(freed_keys == 2, "both keys freed");
    check(freed_vals == 2, "both vals freed");
}

int main(void) {
    test_key_val_free();
    puts("All hash free tests passed.");
    return 0;
}
