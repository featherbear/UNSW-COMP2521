//
// Created by Andrew on 24/01/2019.
//
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifndef UNSW_COMP2521_QUICKSORT_H
#define UNSW_COMP2521_QUICKSORT_H

unsigned int insertionThreshold = 0;
unsigned int postInsertionThreshold = 0;

static void sort_insertion(Item items[], size_t lo, size_t hi);

static void sort_insertion(Item items[], size_t lo, size_t hi) {
    for (size_t i = lo + 1; i <= hi; i++) {
        Item item = items[i];
        size_t j = i;
        for (; j > lo; j--) {
            if (!less(item, items[j - 1])) break;
            else items[j] = items[j - 1];
        }
        items[j] = item;
    }
}

enum mode {
    MODE_NAIVE = 1,  // 0b0001
    MODE_MEDIAN = 2, // 0b0010
    MODE_RANDOM = 4, // 0b0100
/*  MODE_QUIET = 8   // 0b1000  */
};

void quicksort_naive(Item items[], size_t size);
void quicksort_median(Item items[], size_t size);
void quicksort_random(Item items[], size_t size);

static void quicksort_naive_do(Item items[], size_t lo, size_t hi);
static void quicksort_median_do(Item items[], size_t lo, size_t hi);
static void quicksort_random_do(Item items[], size_t lo, size_t hi);
static size_t partition(Item items[], size_t lo, size_t hi);

void quicksort_naive(Item items[], size_t size) {
    quicksort_naive_do(items, 0, size - 1);
    if (postInsertionThreshold) sort_insertion(items, 0, size - 1);
}

void quicksort_median(Item items[], size_t size) {
    quicksort_median_do(items, 0, size - 1);
    if (postInsertionThreshold) sort_insertion(items, 0, size - 1);
}

void quicksort_random(Item items[], size_t size) {
    srand((unsigned int) time(NULL));
    quicksort_random_do(items, 0, size - 1);
    if (postInsertionThreshold) sort_insertion(items, 0, size - 1);
}
static void quicksort_naive_do(Item items[], size_t lo, size_t hi) {
    if (hi <= lo) return;

    size_t n_items = hi - lo + 1;
    if (n_items < insertionThreshold || n_items < postInsertionThreshold) {
        if (n_items < insertionThreshold) sort_insertion(items, lo, hi);
    } else {
        size_t part = partition(items, lo, hi);
        quicksort_naive_do(items, lo, (part == 0) ? 0 : part - 1);
        quicksort_naive_do(items, part + 1, hi);
    }
}

static void quicksort_median_do(Item items[], size_t lo, size_t hi) {
    if (hi <= lo) return;

    size_t mid = (hi + lo) / 2;

    // Median sort
    if (more(items[lo], items[mid])) swap_idx(items, lo, mid);
    if (more(items[mid], items[hi])) swap_idx(items, mid, hi);
    if (more(items[lo], items[mid])) swap_idx(items, lo, mid);
    swap_idx(items, lo, mid);

    size_t n_items = hi - lo + 1;
    if (n_items < insertionThreshold || n_items < postInsertionThreshold) {
        if (n_items < insertionThreshold) sort_insertion(items, lo, hi);
    } else {
        size_t part = partition(items, lo, hi);
        quicksort_median_do(items, lo, (part == 0) ? 0 : (part - 1));
        quicksort_median_do(items, part + 1, hi);
    }
}


static void quicksort_random_do(Item items[], size_t lo, size_t hi) {
    // > but the partitioning algorithm assumes that the pivot is at the leftmost element!
    // :: Easy, just swap the pivot we picked with the leftmost element, then the leftmost element becomes the pivot.

    if (hi <= lo) return;

    size_t randIndex = lo + (((size_t) rand() % (hi - lo + 1)));
    swap_idx(items, randIndex, lo);

    size_t n_items = hi - lo + 1;
    if (n_items < insertionThreshold || n_items < postInsertionThreshold) {
        if (n_items < insertionThreshold) sort_insertion(items, lo, hi);
    } else {
        size_t part = partition(items, lo, hi);
        quicksort_random_do(items, lo, (part == 0) ? 0 : (part - 1));
        quicksort_random_do(items, part + 1, hi);
    }
}


static size_t partition(Item items[], size_t lo, size_t hi) {
    // Assume pivot as left-most

    Item v = items[lo];
    size_t i = lo + 1, j = hi;

    while (true) {
        while (less(items[i], v) && i < j) i++;
        while (less(v, items[j]) && i < j) j--;
        if (i == j) break;
        swap_idx(items, i, j);
    }

    j = less(items[i], v) ? i : i - 1;
    swap_idx(items, lo, j);
    return j;
}

#endif //UNSW_COMP2521_QUICKSORT_H
