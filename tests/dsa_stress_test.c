#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define QUEUE_IMPLEMENTATION
#define HEAP_IMPLEMENTATION
#define HASH_IMPLEMENTATION
#include "queue.h"
#include "heap.h"
#include "hash.h"

#define QUEUE_ITEMS 100000
#define HEAP_ITEMS 100000
#define HASH_ITEMS 10000
#define HASH_LOOKUPS 10000

static void check(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        exit(EXIT_FAILURE);
    }
}

static void debug_message(const char *message) {
    printf("[debug] %s\n", message);
    fflush(stdout);
}

static void debug_progress(const char *phase, int current, int total) {
    if (current == 0 || current % 10000 == 0 || current == total - 1) {
        printf("[debug] %s: %d/%d\n", phase, current + 1, total);
        fflush(stdout);
    }
}

static int int_cmp(const void *first, const void *second) {
    int left = *(const int *)first;
    int right = *(const int *)second;
    return (left > right) - (left < right);
}

static void stress_queue(void) {
    queue_t queue;
    int value;
    int expected = 0;

    check(q_init(&queue, sizeof(int), NULL), "queue initialization");
    for (int i = 0; i < QUEUE_ITEMS; ++i) {
        check(enqueue(&queue, &i), "queue enqueue");
        if (i % 3 == 2) {
            check(dequeue(&queue, &value), "queue dequeue during fill");
            check(value == expected++, "queue FIFO order during fill");
        }
    }
    while (!q_isempty(&queue)) {
        check(dequeue(&queue, &value), "queue final dequeue");
        check(value == expected++, "queue FIFO order at drain");
    }
    check(expected == QUEUE_ITEMS, "queue item count");
    check(q_size(&queue) == 0, "queue empty after stress test");
    q_free(&queue);
    puts("Queue stress test passed.");
}

static void stress_heap(void) {
    hp_t heap;
    int value;
    int previous = HEAP_ITEMS;

    check(hp_init(&heap, sizeof(int), int_cmp, NULL), "heap initialization");
    for (int i = 0; i < HEAP_ITEMS; ++i) {
        int generated = (i * 7919) % HEAP_ITEMS;
        check(hp_insert(&heap, &generated), "heap insertion");
    }
    for (int i = 0; i < HEAP_ITEMS; ++i) {
        check(hp_popmax(&heap, &value), "heap pop");
        check(value <= previous, "max heap order");
        previous = value;
    }
    check(hp_popmax(&heap, &value) == 0, "heap empty after stress test");
    hp_free(&heap);
    puts("Heap stress test passed.");
}

static double stress_hash(size_t buckets) {
    hash_t hash;
    int value;
    clock_t started;
    clock_t finished;

    printf("[debug] hash test starting: %zu buckets\n", buckets);
    fflush(stdout);
    debug_message("hash initialization starting");
    check(ht_init(&hash, sizeof(int), sizeof(int), buckets, NULL, NULL), "hash initialization");
    debug_message("hash initialization complete");
    for (int i = 0; i < HASH_ITEMS; ++i) {
        value = i * 3;
        check(ht_put(&hash, &i, &value), "hash insertion");
        debug_progress("hash insertion", i, HASH_ITEMS);
    }
    debug_message("hash insertion complete");

    debug_message("hash lookup starting");
    started = clock();
    for (int i = 0; i < HASH_LOOKUPS; ++i) {
        int key = (i * 48271) % HASH_ITEMS;
        check(ht_get(&hash, &key, &value), "hash lookup");
        check(value == key * 3, "hash lookup value");
        debug_progress("hash lookup", i, HASH_LOOKUPS);
    }
    finished = clock();
    debug_message("hash lookup complete");

    debug_message("hash removal starting");
    for (int i = 0; i < HASH_ITEMS; i += 2) {
        check(ht_rem(&hash, &i), "hash removal");
        debug_progress("hash removal", i / 2, HASH_ITEMS / 2);
    }
    debug_message("hash removal complete");

    debug_message("hash removal verification starting");
    for (int i = 0; i < HASH_ITEMS; ++i) {
        int found = ht_get(&hash, &i, &value);
        check(found == (i % 2), "hash removal visibility");
        debug_progress("hash removal verification", i, HASH_ITEMS);
    }
    debug_message("hash removal verification complete");

    debug_message("hash cleanup starting");
    ht_free(&hash);
    debug_message("hash cleanup complete");
    return (double)(finished - started) / CLOCKS_PER_SEC;
}

int main(void) {
    double one_bucket_time;
    double two_buckets_time;
    double five_buckets_time;

    stress_queue();
    stress_heap();
    one_bucket_time = stress_hash(1);
    two_buckets_time = stress_hash(2);
    five_buckets_time = stress_hash(5);

    printf("Hash stress test passed.\n"
           "Hash lookup time: 1 bucket = %.6f s, 2 buckets = %.6f s, 5 buckets = %.6f s\n",
           one_bucket_time, two_buckets_time, five_buckets_time);
    puts("All stress tests passed.");
    return EXIT_SUCCESS;
}
