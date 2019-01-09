#include <stdbool.h>
#include <stdlib.h>

#include "heap_min_ordered.h"

bool heap_min_ordered_p_rec (node *tree, int min);
/**
 * Returns 1 if the tree-based heap is in min-heap order.
 * You may assume an empty tree is in min-heap order.
 * You may assume a single node is in min-heap order.
 */
bool heap_min_ordered_p (node *tree)
{
	if (!tree) return true;
	return heap_min_ordered_p_rec(tree, tree->item);
}

bool heap_min_ordered_p_rec (node *tree, int min)
{
	if (!tree) return true;
	if (tree->item < min) return false;
	return heap_min_ordered_p_rec(tree->left, tree->item) && heap_min_ordered_p_rec(tree->right, tree->item);
}
