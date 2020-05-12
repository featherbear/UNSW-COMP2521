#include <stdbool.h>
#include <stdlib.h>

#ifndef HEAP_MIN_ORDERED_H_
#define HEAP_MIN_ORDERED_H_

typedef struct node node;
struct node {
	int   item;
	node *left, *right;
};

// This function should return true
// if the tree is in min heap order,
// and false otherwise
bool heap_min_ordered_p (node *tree);

#endif /* !defined(HEAP_MIN_ORDERED_H_) */
