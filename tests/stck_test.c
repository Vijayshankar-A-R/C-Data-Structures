#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACK_IMPLEMENTATION
#include "stack.h"


static void check(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        exit(1);
    }
}

typedef struct {
    int id;
    char name[16];
} person_t;

static void print_free(void *ptr) {
    person_t *p = (person_t *)ptr;
    printf("[dbg] freeing person: %d %s\n", p->id, p->name);
}

static void test_int_stack(void) {
    stack_t st;
    int values[] = {10, 20, 30, 40};
    int out = 0;

    printf("[dbg] testing int stack with NULL free callback\n");
    check(stack_init(&st, sizeof(int), NULL) == 1, "stack_init for ints");
    check(stack_isempty(&st) == 1, "new int stack is empty");

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        check(stack_push(&st, &values[i]) == 1, "push int");
    }

    check(stack_peek(&st, &out) == 1 && out == 40, "peek top int");
    check(stack_pop(&st, &out) == 1 && out == 40, "pop top int");
    check(stack_pop(&st, &out) == 1 && out == 30, "pop next int");
    check(stack_pop(&st, &out) == 1 && out == 20, "pop next int");
    check(stack_pop(&st, &out) == 1 && out == 10, "pop last int");
    check(stack_isempty(&st) == 1, "stack empty after pops");
    check(stack_pop(&st, &out) == 0, "pop from empty stack fails");

    stack_free(&st);
}

static void test_struct_stack(void) {
    stack_t st;
    person_t people[] = {{1, "Ada"}, {2, "Bob"}, {3, "Cia"}};
    person_t out;

    printf("[dbg] testing struct stack with print callback\n");
    check(stack_init(&st, sizeof(person_t), print_free) == 1, "stack_init for structs");
    check(stack_push(&st, &people[0]) == 1, "push struct 1");
    check(stack_push(&st, &people[1]) == 1, "push struct 2");
    check(stack_push(&st, &people[2]) == 1, "push struct 3");

    check(stack_peek(&st, &out) == 1 && out.id == 3 && strcmp(out.name, "Cia") == 0, "peek struct");
    check(stack_pop(&st, &out) == 1 && out.id == 3 && strcmp(out.name, "Cia") == 0, "pop struct");

    printf("[dbg] freeing remaining struct elements via stack_free\n");
    stack_free(&st);
}

int main(void) {
    test_int_stack();
    test_struct_stack();
    puts("All stack tests passed.");
    return 0;
}
