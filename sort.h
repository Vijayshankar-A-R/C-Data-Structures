// sort.h
#ifndef SORT_H
#define SORT_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(void *base, size_t nmemb, size_t size,
                    int (*cmp)(const void *, const void *));
void selection_sort(void *base, size_t nmemb, size_t size,
                    int (*cmp)(const void *, const void *));
void merge_sort(void *base, size_t nmemb, size_t size,
                int (*cmp)(const void *, const void *));
void bubble_sort(void *base, size_t nmemb, size_t size,
                 int (*cmp)(const void *, const void *));
void quick_sort(void *base, size_t nmemb, size_t size,
                int (*cmp)(const void *, const void *));
#endif // SORT_H

#define ELE_ADDR(arr, sz, idx)                                                 \
    ((void *)((unsigned char *)(arr) + ((size_t)(idx) * (size_t)(sz))))

static size_t bsearch_left(void *base, size_t nmemb, size_t size,
                           int (*cmp)(const void *, const void *),
                           const void *key) {
    size_t l, r, m;
    l = 0;
    r = nmemb;
    while (l < r) {
        m = (l + r) / 2;
        if (cmp(ELE_ADDR(base, size, m), key) < 0)
            l = m + 1;
        else
            r = m;
    }
    return l;
}

#ifdef INSERT_SORT_IMPL

void insertion_sort(void *base, size_t nmemb, size_t size,
                    int (*cmp)(const void *, const void *)) {
    if (!base || !cmp || nmemb < 2 || size == 0)
        return;
    void *tmp = malloc(size);
    if (!tmp)
        return;

    size_t sorted = 1;

    while (sorted != nmemb) {
        size_t i = bsearch_left(
            base, sorted, size, cmp,
            ELE_ADDR(base, size, sorted)); // Search in the sorted subsequence
        memcpy(tmp, ELE_ADDR(base, size, sorted), size);
        memmove(ELE_ADDR(base, size, i + 1), ELE_ADDR(base, size, i),
                (sorted - i) * size);
        memcpy(ELE_ADDR(base, size, i), tmp, size);
        sorted++;
    }

    free(tmp);
}

#endif // INSERT_SORT_IMPL

#ifdef SELECT_SORT_IMPL

void selection_sort(void *base, size_t nmemb, size_t size,
                    int (*cmp)(const void *, const void *)) {
    if (!base || !cmp || size == 0 || nmemb < 2)
        return;
    void *tmp = malloc(size);
    if (!tmp)
        return;

    for (size_t i = 0; i < nmemb - 1; ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < nmemb; ++j)
            if (cmp(ELE_ADDR(base, size, j), ELE_ADDR(base, size, min_idx)) < 0)
                min_idx = j;
        // swap arr[i] and arr[min_idx]
        memcpy(tmp, ELE_ADDR(base, size, i), size);
        memcpy(ELE_ADDR(base, size, i), ELE_ADDR(base, size, min_idx), size);
        memcpy(ELE_ADDR(base, size, min_idx), tmp, size);
    }

    free(tmp);
}

#endif // SELECT_SORT_IMPL

#ifdef MERGE_SORT_IMPL

static void merge_arr(void *base, size_t size, size_t left, size_t middle,
                      size_t right, int (*cmp)(const void *, const void *)) {
    size_t m, n;
    m = middle - left;
    n = right - middle;
    void *left_part = malloc(m * size);
    if (!left_part)
        return;
    void *right_part = malloc(n * size);
    if (!right_part)
        return free(left_part);

    memcpy(left_part, ELE_ADDR(base, size, left), m * size);
    memcpy(right_part, ELE_ADDR(base, size, middle), n * size);

    size_t i, j, k;
    i = 0;
    j = 0;
    k = left;

    while (i < m && j < n) {
        if (cmp(ELE_ADDR(left_part, size, i), ELE_ADDR(right_part, size, j)) <=
            0) {
            memcpy(ELE_ADDR(base, size, k), ELE_ADDR(left_part, size, i), size);
            ++i;
        } else {
            memcpy(ELE_ADDR(base, size, k), ELE_ADDR(right_part, size, j),
                   size);
            ++j;
        }
        ++k;
    }

    while (i < m) {
        memcpy(ELE_ADDR(base, size, k), ELE_ADDR(left_part, size, i), size);
        ++i;
        ++k;
    }

    while (j < n) {
        memcpy(ELE_ADDR(base, size, k), ELE_ADDR(right_part, size, j), size);
        ++j;
        ++k;
    }

    free(left_part);
    free(right_part);
}

void merge_sort(void *base, size_t nmemb, size_t size,
                int (*cmp)(const void *, const void *)) {
    if (!base || !cmp || size == 0)
        return;
    size_t w, l, r, m;
    w = 1;

    while (w < nmemb) {
        l = 0;

        while (l < nmemb) {
            m = (l + w) < nmemb ? (l + w) : nmemb; // min(l + w, nmemb)
            r = (l + 2 * w) < nmemb ? (l + 2 * w)
                                    : nmemb; // min(l + 2 * w, nmemb)

            merge_arr(base, size, l, m, r, cmp);

            l += 2 * w;
        }
        w <<= 1;
    }
}

#endif // MERGE_SORT_IMPL

#ifdef BUBBLE_SORT_IMPL

void bubble_sort(void *base, size_t nmemb, size_t size,
                 int (*cmp)(const void *, const void *)) {
    if (!base || !cmp || size == 0)
        return;

    void *tmp = malloc(size);
    if (!tmp)
        return;

    for (size_t i = 0; i < nmemb; ++i) {
        int swapped = 0;

        for (size_t j = 0; j < nmemb - i - 1; ++j) {
            if (cmp(ELE_ADDR(base, size, j), ELE_ADDR(base, size, j + 1)) > 0) {
                memcpy(tmp, ELE_ADDR(base, size, j), size);
                memcpy(ELE_ADDR(base, size, j), ELE_ADDR(base, size, j + 1),
                       size);
                memcpy(ELE_ADDR(base, size, j + 1), tmp, size);
                swapped = 1;
            }
        }

        if (!swapped)
            break;
    }
}

#endif // BUBBLE_SORT_IMPL

#ifdef QUICK_SORT_IMPL

static size_t partition(void *base, size_t size, size_t low, size_t high,
                        int (*cmp)(const void *, const void *)) {
    void *pivot = ELE_ADDR(base, size, high);
    size_t i = low;

    void *tmp = malloc(size);
    if (!tmp)
        return 0x0BAD;

    for (size_t j = low; j < high; ++j) {
        if (cmp(ELE_ADDR(base, size, j), pivot) <= 0) {
            memcpy(tmp, ELE_ADDR(base, size, i), size);
            memcpy(ELE_ADDR(base, size, i), ELE_ADDR(base, size, j), size);
            memcpy(ELE_ADDR(base, size, j), tmp, size);
            ++i;
        }
    }

    memcpy(tmp, ELE_ADDR(base, size, i), size);
    memcpy(ELE_ADDR(base, size, i), ELE_ADDR(base, size, high), size);
    memcpy(ELE_ADDR(base, size, high), tmp, size);

    free(tmp);
    return i;
}

static void __qsort_rec(void *base, size_t size, size_t low, size_t high,
                        int (*cmp)(const void *, const void *)) {
    if (low >= high)
        return;
    size_t p_idx = partition(base, size, low, high, cmp);
    if (p_idx == 0x0BAD)
        return;
    if (p_idx > low)
        __qsort_rec(base, size, low, p_idx - 1, cmp);
    if (p_idx < high)
        __qsort_rec(base, size, p_idx + 1, high, cmp);
}

void quick_sort(void *base, size_t nmemb, size_t size,
                int (*cmp)(const void *, const void *)) {
    if (!base || !cmp || size == 0 || nmemb < 2)
        return;
    __qsort_rec(base, size, 0, nmemb - 1, cmp);
}

#endif // QUICK_SORT_IMPL
#undef ELE_ADDR
