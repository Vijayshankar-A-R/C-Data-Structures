#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HEAP_IMPLEMENTATION
#include "heap.h"

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

static void test_int_heap(void) {
    hp_t h;
    int vals[] = {10, 5, 15, 3, 7};
    int out;

    check(hp_init(&h, sizeof(int), int_cmp, NULL) == 1, "hp_init for ints");

    for (size_t i = 0; i < sizeof(vals)/sizeof(vals[0]); ++i) {
        check(hp_insert(&h, &vals[i]) == 1, "hp_insert int");
    }

    check(hp_peek(&h, &out) == 1 && out == 15, "peek returns max 15");

    check(hp_popmax(&h, &out) == 1 && out == 15, "popmax returns 15");
    check(hp_popmax(&h, &out) == 1 && out == 10, "popmax returns 10");
    check(hp_popmax(&h, &out) == 1 && out == 7, "popmax returns 7");
    check(hp_popmax(&h, &out) == 1 && out == 5, "popmax returns 5");
    check(hp_popmax(&h, &out) == 1 && out == 3, "popmax returns 3");

    hp_free(&h);
}

static void test_struct_heap(void) {
    hp_t h;
    person_t people[] = {{1, "Ada"}, {2, "Bob"}, {3, "Cia"}};
    person_t out;

    check(hp_init(&h, sizeof(person_t), person_cmp, print_free) == 1, "hp_init for structs");

    for (size_t i = 0; i < sizeof(people)/sizeof(people[0]); ++i) {
        check(hp_insert(&h, &people[i]) == 1, "hp_insert struct");
    }

    check(hp_popmax(&h, &out) == 1 && out.id == 3, "popmax returns id 3");
    check(hp_popmax(&h, &out) == 1 && out.id == 2, "popmax returns id 2");

    printf("[dbg] freeing remaining heap via hp_free\n");
    hp_free(&h);
}

int main(void) {
    test_int_heap();
    test_struct_heap();
    puts("All heap tests passed.");
    return 0;
}
