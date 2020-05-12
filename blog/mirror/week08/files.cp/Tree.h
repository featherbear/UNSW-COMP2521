#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stdlib.h>

#include "Item.h"

enum tree_strategy {
	NO_REBALANCE,   // no rebalancing strategy
	REBALANCE_1,    // rebalance after every insertion
	REBALANCE_100,  // rebalance after every 100 insertions
	REBALANCE_1000, // rebalance after every 1000 insertions
	RANDOMISED,     // used randomInsertion
	SPLAY,          // use splay insertion (and search)
};

typedef struct tree *Tree;

// Initialises a tree.
Tree tree_new (enum tree_strategy);

// Insert an item into a binary tree and balance the tree,
// depending on the defined rebalance strategy.
void tree_insert (Tree t, Item i);

// Count the number of items in the tree.
size_t tree_count(Tree t);

// Search the tree for a specified key.
bool tree_search(Tree t, Key k);

// Calculate the height of the tree.
ssize_t tree_height(Tree t);

// Free the memory associated with the tree.
void tree_destroy(Tree t);

// Display the tree.
void tree_display(Tree t);

// Print out some statistics about the tree.
void tree_print_stats(Tree t);

#endif
