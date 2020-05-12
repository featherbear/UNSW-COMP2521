////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Heaps!
//
// 2018-12-26	Jashank Jeremy <jashankj@cse.unsw.edu.au>

#include <stdbool.h>
#include <stdlib.h>

#include "item.h"

#ifndef CS2521__HEAP_H_
#define CS2521__HEAP_H_

/**
 * A heap is a binary tree which has a top-to-bottom ordering of values
 * (the "heap ordering" property, where for all nodes, subtrees are
 * exactly less than, for a maximum heap, or exactly greater than, for a
 * maximum heap, the node's value), and which fills each level of the
 * tree (the "complete tree" property).
 */
typedef struct heap *Heap;

/** Orderings of values in a heap. */
typedef enum heap_ordering {
	HEAP_ORD_MINIMUM,	/**< The minimum value at the root. */
	HEAP_ORD_MAXIMUM,	/**< The maximum value at the root. */
} heap_ordering;

/** Create a new, empty Heap. */
Heap heap_new (heap_ordering);

/** Destroy a Heap, releasing all resources associated with it. */
void heap_drop (Heap);

/** Add an item to a Heap. */
void heap_insert (Heap, Item);

/** Remove an item from the root of a Heap. */
Item heap_remove (Heap);

/** Get the number of items in a Heap. */
size_t heap_size (Heap);

#endif // !defined(CS2521__HEAP_H_)
