//
// Created by Andrew on 24/01/2019.
//

#ifndef UNSW_COMP2521_ITEM_H
#define UNSW_COMP2521_ITEM_H

#include <stdio.h>

typedef int Item;

#define less(A, B) (A < B)
#define more(A, B) (A > B)

void printArray(Item *items, size_t n_items, char *start, char *end, char *sep);
#define printArrayLines(items, n_items) printArray(items, n_items, "", "\n", "\n")
#define printArrayInline(items, n_items) printArray(items, n_items, "", "", " ")

void swap_idx(Item items[], size_t a, size_t b);

#endif //UNSW_COMP2521_ITEM_H
