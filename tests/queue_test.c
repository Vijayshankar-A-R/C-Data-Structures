#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

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

static void test_int_queue(void) {
    queue_t q;
    int values[] = {10, 20, 30, 40};
    int out;

    check(q_init(&q, sizeof(int), NULL) == 1, "q_init for ints");
    check(q_isempty(&q) == 1, "new int queue is empty");
    check(q_size(&q) == 0, "new int queue has size 0");

    check(enqueue(&q, &values[0]) == 1, "enqueue 10");
    check(enqueue(&q, &values[1]) == 1, "enqueue 20");
    check(enqueue(&q, &values[2]) == 1, "enqueue 30");
    check(q_size(&q) == 3, "size after three enqueues");
    check(q_isempty(&q) == 0, "queue not empty after enqueues");

    check(dequeue(&q, &out) == 1 && out == 10, "dequeue returns first item");
    check(dequeue(&q, &out) == 1 && out == 20, "dequeue returns second item");
    check(enqueue(&q, &values[3]) == 1, "enqueue 40 after dequeue");
    check(q_size(&q) == 2, "size after dequeue and enqueue");

    check(dequeue(&q, &out) == 1 && out == 30, "dequeue returns third item");
    check(dequeue(&q, &out) == 1 && out == 40, "dequeue returns fourth item");
    check(q_isempty(&q) == 1, "queue empty at end");
    check(dequeue(&q, &out) == 0, "dequeue from empty queue fails");

    q_free(&q);
}

static void test_struct_queue(void) {
    queue_t q;
    person_t values[] = {{1, "Ada"}, {2, "Bob"}, {3, "Cia"}};
    person_t out;

    check(q_init(&q, sizeof(person_t), print_free) == 1, "q_init for structs");
    check(enqueue(&q, &values[0]) == 1, "enqueue struct Ada");
    check(enqueue(&q, &values[1]) == 1, "enqueue struct Bob");
    check(enqueue(&q, &values[2]) == 1, "enqueue struct Cia");
    check(q_size(&q) == 3, "struct queue size after enqueue");

    check(dequeue(&q, &out) == 1 && out.id == 1 && strcmp(out.name, "Ada") == 0, "dequeue struct Ada");
    check(dequeue(&q, &out) == 1 && out.id == 2 && strcmp(out.name, "Bob") == 0, "dequeue struct Bob");
    check(q_isempty(&q) == 0, "struct queue not empty after partial dequeues");
    check(q_size(&q) == 1, "one element should remain before q_free");

    printf("[dbg] freeing remaining struct elements via q_free\n");
    q_free(&q);
}

int main(void) {
    test_int_queue();
    test_struct_queue();
    puts("All queue tests passed.");
    return 0;
}
