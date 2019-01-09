#include <stdbool.h>
#include <stdlib.h>

#include "heap_min.h"
#include <string.h>
/**
 * Return `true` if the array heap with a specified number of items is
 * in heap order.  You must assume that the heap items are in indexes
 * 1..heap_size, and that index 0 is empty and not used to store items.
 */
bool heap_min_p(int heap[], size_t heap_size) {
/*
           1
         2   4
       5  3
*/
//    for (int i = 1; i < heap_size / 2; i++) {
    for (int i = 1; i < heap_size; i++) {
        int lchild = 2 * i;
        int rchild = lchild + 1;
        if (lchild <= heap_size && heap[lchild] < heap[i]) return false;
        if (rchild <= heap_size && heap[rchild] < heap[i]) return false;
    }
// 2 -> heap_size
// i/2
    return true;

}
