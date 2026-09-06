#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

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

static void print_free(void *v) {
	person_t *p = (person_t *)v;
	printf("[dbg] freeing person: %d %s\n", p->id, p->name);
}

static void test_int_list(void) {
    list_t list;
    int values[] = {10, 20, 30, 40, 50};
    int out = 0;

    check(list_init(&list, 2, sizeof(int), NULL) == 1, "list_init for ints");
    check(list_size(&list) == 0, "new int list size is 0");

    check(add(&list, &values[0]) == 1, "add first int");
    check(add(&list, &values[1]) == 1, "add second int");
    check(add(&list, &values[2]) == 1, "add third int");
    check(list_size(&list) == 3, "size after adds");

    check(get(&list, 0, &out) == 1 && out == 10, "get index 0");
    check(get(&list, 1, &out) == 1 && out == 20, "get index 1");
    check(get(&list, 2, &out) == 1 && out == 30, "get index 2");

    check(set(&list, 1, &values[3]) == 1, "set index 1 to 40");
    check(get(&list, 1, &out) == 1 && out == 40, "get updated value at index 1");

    check(set(&list, 3, &values[4]) == 1, "append via set at size");
    check(list_size(&list) == 4, "size after append with set");
    check(get(&list, 3, &out) == 1 && out == 50, "get appended value");

    check(get(&list, 4, &out) == 0, "out-of-range get fails");
    check(set(&list, 5, &values[0]) == 0, "out-of-range set fails");

    list_free(&list);
}

static void test_struct_list(void) {
    list_t list;
    person_t people[] = {{1, "Ada"}, {2, "Bob"}, {3, "Cia"}};
    person_t out;

    check(list_init(&list, 2, sizeof(person_t), print_free) == 1, "list_init for structs");
    check(add(&list, &people[0]) == 1, "add struct 1");
    check(add(&list, &people[1]) == 1, "add struct 2");
    check(add(&list, &people[2]) == 1, "add struct 3");
    check(list_size(&list) == 3, "struct list size after adds");

    check(get(&list, 0, &out) == 1 && out.id == 1 && strcmp(out.name, "Ada") == 0, "get struct 0");
    check(get(&list, 2, &out) == 1 && out.id == 3 && strcmp(out.name, "Cia") == 0, "get struct 2");

    person_t replacement = {4, "Dana"};
    check(set(&list, 1, &replacement) == 1, "replace struct at index 1");
    check(get(&list, 1, &out) == 1 && out.id == 4 && strcmp(out.name, "Dana") == 0, "get replaced struct");

    list_free(&list);
}

int main(void) {
    test_int_list();
    test_struct_list();
    puts("All list tests passed.");
    return 0;
}
