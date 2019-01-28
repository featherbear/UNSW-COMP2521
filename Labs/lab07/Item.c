//
// Created by Andrew on 24/01/2019.
//

#include "Item.h"

void swap_idx(Item items[], size_t a, size_t b) {
    Item t = items[b];
    items[b] = items[a];
    items[a] = t;
}

void printArray(Item *items, size_t n_items, char *start, char *end, char *sep) {
    printf("%s", start);
    for (size_t i = 0; i < n_items - 1; i++) printf("%d%s", items[i], sep);
    printf("%d%s", items[n_items - 1], end);
}
