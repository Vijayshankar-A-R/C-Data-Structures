#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INSERT_SORT_IMPL
#define SELECT_SORT_IMPL
#define MERGE_SORT_IMPL
#define BUBBLE_SORT_IMPL
#define QUICK_SORT_IMPL
#include "sort.h"

typedef void (*sort_fn)(void *, size_t, size_t,
                        int (*)(const void *, const void *));

typedef struct {
    const char *name;
    sort_fn sort;
} sort_case_t;

typedef struct {
    int key;
    char label[8];
} record_t;

static void check(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        exit(1);
    }
}

static int int_cmp(const void *a, const void *b) {
    int left = *(const int *)a;
    int right = *(const int *)b;
    return (left > right) - (left < right);
}

static int record_cmp(const void *a, const void *b) {
    const record_t *left = a;
    const record_t *right = b;
    return (left->key > right->key) - (left->key < right->key);
}

static void print_ints(const char *label, const int *values, size_t count) {
    printf("%s", label);
    for (size_t i = 0; i < count; ++i)
        printf(" %d", values[i]);
    putchar('\n');
}

static void print_records(const char *label, const record_t *values,
                          size_t count) {
    printf("%s", label);
    for (size_t i = 0; i < count; ++i)
        printf(" {%d, %s}", values[i].key, values[i].label);
    putchar('\n');
}

static void test_int_sort(const sort_case_t *sort_case) {
    int values[] = {7, -2, 7, 4, 0, 9, -5, 4};
    int expected[] = {-5, -2, 0, 4, 4, 7, 7, 9};

    printf("\n[%s] integer sort\n", sort_case->name);
    print_ints("before:", values, sizeof(values) / sizeof(values[0]));
    sort_case->sort(values, sizeof(values) / sizeof(values[0]), sizeof(int),
                    int_cmp);
    print_ints("after: ", values, sizeof(values) / sizeof(values[0]));
    check(memcmp(values, expected, sizeof(values)) == 0,
          sort_case->name);
}

static void test_record_sort(const sort_case_t *sort_case) {
    record_t values[] = {{3, "three"}, {1, "one"}, {2, "two"},
                         {1, "uno"}};
    int expected_keys[] = {1, 1, 2, 3};

    printf("[%s] record sort\n", sort_case->name);
    print_records("before:", values, sizeof(values) / sizeof(values[0]));
    sort_case->sort(values, sizeof(values) / sizeof(values[0]), sizeof(values[0]),
                    record_cmp);
    print_records("after: ", values, sizeof(values) / sizeof(values[0]));
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
        check(values[i].key == expected_keys[i], sort_case->name);
}

static void test_small_inputs(const sort_case_t *sort_case) {
    int value = 42;

    sort_case->sort(NULL, 0, sizeof(int), int_cmp);
    sort_case->sort(&value, 1, sizeof(int), int_cmp);
    check(value == 42, sort_case->name);
}

int main(void) {
    const sort_case_t cases[] = {
        {"insertion_sort", insertion_sort},
        {"selection_sort", selection_sort},
        {"merge_sort", merge_sort},
        {"bubble_sort", bubble_sort},
        {"quick_sort", quick_sort},
    };

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        test_int_sort(&cases[i]);
        test_record_sort(&cases[i]);
        test_small_inputs(&cases[i]);
    }

    puts("All sort tests passed.");
    return 0;
}