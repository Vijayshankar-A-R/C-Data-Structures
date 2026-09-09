#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BST_IMPLEMENTATION
#include "bst.h"

static void check(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        exit(1);
    }
}

static int int_cmp(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

static void test_int_bst(void) {
    bst_t tree;
    int vals[] = {10, 5, 15, 3, 7};

    check(bst_init(&tree, sizeof(int), int_cmp, NULL) == 1, "bst_init for ints");

    check(bst_contains(&tree, &vals[0]) == 0, "empty tree does not contain 10");

    for (size_t i = 0; i < sizeof(vals)/sizeof(vals[0]); ++i) {
        check(bst_insert(&tree, &vals[i]) == 1, "bst_insert returns 1");
    }

    check(bst_contains(&tree, &vals[0]) == 1, "tree contains 10");
    check(bst_contains(&tree, &vals[1]) == 1, "tree contains 5");
    check(bst_contains(&tree, &vals[2]) == 1, "tree contains 15");

    int x = 7;
    check(bst_contains(&tree, &x) == 1, "tree contains 7");

    int y = 20;
    check(bst_contains(&tree, &y) == 0, "tree does not contain 20");

    check(bst_delete(&tree, &vals[1]) == 1, "bst_delete returns 1 for existing element");
    check(bst_contains(&tree, &vals[1]) == 0, "5 removed from tree");

    bst_free(&tree);
}

typedef struct {
    int id;
    char name[16];
} person_t;

static void print_free(void *ptr) {
    person_t *p = (person_t *)ptr;
    printf("[dbg] freeing person: %d %s\n", p->id, p->name);
}

static int person_cmp(const void *a, const void *b) {
    const person_t *pa = a;
    const person_t *pb = b;
    if (pa->id < pb->id) return -1;
    if (pa->id > pb->id) return 1;
    return 0;
}

static void test_struct_free(void) {
    bst_t tree;
    person_t people[] = {
        {1, "Ada"},
        {2, "Bob"},
        {3, "Cia"}
    };

    check(bst_init(&tree, sizeof(person_t), person_cmp, print_free) == 1, "bst_init for structs");

    for (size_t i = 0; i < sizeof(people)/sizeof(people[0]); ++i) {
        check(bst_insert(&tree, &people[i]) == 1, "bst_insert struct returns 1");
    }

    printf("[dbg] freeing struct BST with print_free callback\n");
    bst_free(&tree);
}

int main(void) {
    test_int_bst();
    test_struct_free();
    puts("All bst tests passed.");
    return 0;
}
