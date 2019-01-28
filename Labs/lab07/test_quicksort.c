#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "Item.h"
#include "_testing.h"
#include "quicksort.h"


bool arrayIsClone(Item *arrA, size_t sizeA, Item *arrB, size_t sizeB);
Item *arrayDoClone(Item *arr, size_t size);

bool arrayIsClone(Item *arrA, size_t sizeA, Item *arrB, size_t sizeB) {
    if (sizeA != sizeB) return false;
    for (size_t i = 0; i < sizeA; i++) if (arrA[i] != arrB[i]) return false;
    return true;
}

Item *arrayDoClone(Item *arr, size_t size) {
    if (!size) return NULL;
    Item *
    new = malloc(size * sizeof(Item));
    for (size_t i = 0; i < size; i++) new[i] = arr[i];
    return new;
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wgnu-folding-constant"
#endif

int main(void) {
    const size_t small_n_items = 10;
    const size_t big_n_items = 200;

    // gcc being annoying, can't put constant ints here? c99 fakeness argh!
    Item small_expected[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Item small_sample[10] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};

    Item big_expected[200] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
                              23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                              43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                              63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
                              83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
                              103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
                              119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
                              135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
                              151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166,
                              167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
                              183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
                              199};
    Item big_sample[200] = {95, 193, 154, 44, 76, 83, 120, 167, 122, 43, 70, 130, 93, 91, 140, 116, 108, 35, 137,
                            58, 198, 176, 106, 119, 71, 48, 99, 20, 150, 112, 37, 40, 54, 184, 152, 123, 42, 160,
                            127, 79, 102, 8, 52, 80, 32, 143, 128, 121, 60, 180, 4, 46, 125, 85, 51, 21, 28, 145,
                            88, 144, 190, 5, 159, 142, 189, 12, 181, 111, 78, 47, 141, 197, 24, 98, 94, 113, 81,
                            166, 11, 25, 147, 64, 157, 45, 6, 199, 194, 101, 90, 179, 36, 39, 15, 148, 0, 107,
                            156, 63, 131, 110, 195, 134, 182, 126, 191, 31, 50, 186, 10, 59, 146, 172, 84, 136,
                            67, 53, 124, 3, 100, 26, 183, 158, 115, 2, 74, 153, 105, 104, 61, 162, 129, 117, 75,
                            27, 29, 96, 9, 38, 155, 72, 170, 73, 103, 178, 86, 69, 17, 16, 175, 192, 135, 34,
                            133, 171, 173, 68, 163, 19, 168, 22, 161, 92, 30, 23, 7, 77, 66, 132, 62, 151, 177,
                            18, 169, 138, 188, 56, 109, 187, 165, 114, 41, 57, 33, 14, 118, 87, 89, 55, 174, 1,
                            97, 13, 164, 49, 149, 185, 139, 82, 196, 65};

    {
        T("Testing an already sorted array");
        {
            A("Naive [Small]");
            Item *array = arrayDoClone(small_expected, small_n_items);
            quicksort_naive(array, small_n_items);
            assert(arrayIsClone(array, small_n_items, small_expected, small_n_items));
            free(array);
            O();
        }
        {
            A("Naive [Big]");
            Item *array = arrayDoClone(big_expected, big_n_items);
            quicksort_naive(array, big_n_items);
            assert(arrayIsClone(array, big_n_items, big_expected, big_n_items));
            free(array);
            O();
        }
        {
            A("Median of Three [Small]");
            Item *array = arrayDoClone(small_expected, small_n_items);
            quicksort_median(array, small_n_items);
            assert(arrayIsClone(array, small_n_items, small_expected, small_n_items));
            free(array);
            O();
        }
        {
            A("Median of Three [Big]");
            Item *array = arrayDoClone(big_expected, big_n_items);
            quicksort_median(array, big_n_items);
            assert(arrayIsClone(array, big_n_items, big_expected, big_n_items));
            free(array);
            O();
        }
        {
            A("Random [Small]");
            Item *array = arrayDoClone(small_expected, small_n_items);
            quicksort_random(array, small_n_items);
            assert(arrayIsClone(array, small_n_items, small_expected, small_n_items));
            free(array);
            O();
        }
        {
            A("Random [Big]");
            Item *array = arrayDoClone(big_expected, big_n_items);
            quicksort_random(array, big_n_items);
            assert(arrayIsClone(array, big_n_items, big_expected, big_n_items));
            free(array);
            O();
        }
    }

    {
        T("Testing an unsorted array");
        {
            A("Naive [Small]");
            Item *array = arrayDoClone(small_sample, small_n_items);
            quicksort_naive(array, small_n_items);
            assert(arrayIsClone(array, small_n_items, small_expected, small_n_items));
            free(array);
            O();
        }
        {
            A("Naive [Big]");
            Item *array = arrayDoClone(big_sample, big_n_items);
            quicksort_naive(array, big_n_items);
            assert(arrayIsClone(array, big_n_items, big_expected, big_n_items));
            free(array);
            O();
        }
        {
            A("Median of Three [Small]");
            Item *array = arrayDoClone(small_sample, small_n_items);
            quicksort_median(array, small_n_items);
            assert(arrayIsClone(array, small_n_items, small_expected, small_n_items));
            free(array);
            O();
        }
        {
            A("Median of Three [Big]");
            Item *array = arrayDoClone(big_sample, big_n_items);
            quicksort_median(array, big_n_items);
            assert(arrayIsClone(array, big_n_items, big_expected, big_n_items));
            free(array);
            O();
        }
        {
            A("Random [Small]");
            Item *array = arrayDoClone(small_sample, small_n_items);
            quicksort_random(array, small_n_items);
            assert(arrayIsClone(array, small_n_items, small_expected, small_n_items));
            free(array);
            O();
        }
        {
            A("Random [Big]");
            Item *array = arrayDoClone(big_sample, big_n_items);
            quicksort_random(array, big_n_items);
            assert(arrayIsClone(array, big_n_items, big_expected, big_n_items));
            free(array);
            O();
        }
    }

}
