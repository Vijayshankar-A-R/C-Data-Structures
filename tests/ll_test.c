#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linklist.h"

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

static void test_int_operations(void) {
    linklist_t list;
    int values[] = {10, 20, 30, 40};
    int out = 0;
    int removed = 0;

    check(ll_init(&list, sizeof(int), NULL) == 1, "ll_init for ints");
    check(ll_isempty(&list) == 1, "new int list is empty");
    check(ll_size(&list) == 0, "new int list has size 0");

    printf("[dbg] inserting values\n");
    check(ll_inserthead(&list, &values[1]) == 1, "insert head 20");
    check(ll_inserthead(&list, &values[0]) == 1, "insert head 10");
    check(ll_inserttail(&list, &values[2]) == 1, "append 30");
    check(ll_insert(&list, 2, &values[3]) == 1, "insert 40 at index 2");

    printf("[dbg] size after insertions: %zu\n", ll_size(&list));
    check(ll_size(&list) == 4, "size after insertions");
    check(ll_gethead(&list, &out) == 1 && out == 10, "get head");
    check(ll_gettail(&list, &out) == 1 && out == 30, "get tail");
    check(ll_getelem(&list, 0, &out) == 1 && out == 10, "get elem[0]");
    check(ll_getelem(&list, 2, &out) == 1 && out == 40, "get elem[2]");

    printf("[dbg] deleting values\n");
    check(ll_deletehead(&list, &removed) == 1 && removed == 10, "delete head");
    check(ll_delete(&list, 1, &removed) == 1 && removed == 40, "delete index 1");
    check(ll_deletetail(&list, &removed) == 1 && removed == 30, "delete tail");

    printf("[dbg] size after deletions: %zu\n", ll_size(&list));
    check(ll_size(&list) == 1, "size after deletions");
    check(ll_gethead(&list, &out) == 1 && out == 20, "head after deletions");
    check(ll_gettail(&list, &out) == 1 && out == 20, "tail after deletions");

    {
        printf("[dbg] starting traversal\n");
        const ll_node *node = list.head;
        int seen = 0;
        while (node) {
            printf("[dbg] visit node %d\n", *(int *)node->data);
            check(*(int *)node->data == 20, "traverse remaining int node");
            ++seen;
            node = node->next;
        }
        printf("[dbg] traversal saw %d nodes\n", seen);
        check(seen == 1, "traversal saw one int node");
    }

    check(ll_getelem(&list, 9, &out) == 0, "out-of-range get should fail");
    check(ll_delete(&list, 9, &removed) == 0, "out-of-range delete should fail");

    ll_free(&list);
}

static void test_struct_operations(void) {
    linklist_t list;
    person_t people[] = {
        {1, "Ada"},
        {2, "Bob"},
        {3, "Cia"}
    };
    person_t out;

    printf("[dbg] starting struct test\n");
    check(ll_init(&list, sizeof(person_t), print_free) == 1, "ll_init for structs");
    check(ll_inserthead(&list, &people[0]) == 1, "insert struct head");
    check(ll_inserthead(&list, &people[1]) == 1, "insert second struct head");
    check(ll_inserttail(&list, &people[2]) == 1, "append struct tail");

    check(ll_size(&list) == 3, "struct size");
    check(ll_gethead(&list, &out) == 1 && out.id == 2 && strcmp(out.name, "Bob") == 0, "get struct head");
    check(ll_gettail(&list, &out) == 1 && out.id == 3 && strcmp(out.name, "Cia") == 0, "get struct tail");
    check(ll_getelem(&list, 1, &out) == 1 && out.id == 1 && strcmp(out.name, "Ada") == 0, "get struct middle");

    printf("[dbg] freeing remaining struct elements via ll_free\n");
    ll_free(&list);
}

int main(void) {
    test_int_operations();
    test_struct_operations();
    puts("All linklist tests passed.");
    return 0;
}

