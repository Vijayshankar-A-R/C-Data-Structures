#include <stdio.h>
#include <stdlib.h>

#define SET_IMPLEMENTATION
#include "set.h"

static void check(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        exit(1);
    }
}

static void test_int_set(void) {
    set_t set;
    int values[] = {10, 20, 30};
    int missing = 40;

    check(set_init(&set, sizeof(int), NULL) == 1, "set_init for ints");

    check(set_insert(&set, &values[0]) == 1, "insert 10");
    check(set_insert(&set, &values[1]) == 1, "insert 20");
    check(set_insert(&set, &values[2]) == 1, "insert 30");
    check(set_contains(&set, &values[0]) == 1, "contains 10");
    check(set_contains(&set, &values[1]) == 1, "contains 20");
    check(set_contains(&set, &values[2]) == 1, "contains 30");
    check(set_contains(&set, &missing) == 0, "does not contain 40");

    check(set_insert(&set, &values[1]) == 0, "reject duplicate 20");
    check(set_remove(&set, &values[1]) == 1, "remove 20");
    check(set_contains(&set, &values[1]) == 0, "20 is absent after remove");
    check(set_remove(&set, &values[1]) == 0, "reject removing absent 20");
    check(set_contains(&set, &values[0]) == 1, "10 remains after remove");
    check(set_contains(&set, &values[2]) == 1, "30 remains after remove");

    set_free(&set);
}

int main(void) {
    test_int_set();
    puts("All set tests passed.");
    return 0;
}
