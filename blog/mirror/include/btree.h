////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Binary trees!
//
// 2018-12-09	Jashank Jeremy <jashankj@cse.unsw.edu.au>

#include <stdbool.h>
#include <stdlib.h>

#include "item.h"

#ifndef CS2521__BTREE_H_
#define CS2521__BTREE_H_

typedef struct btree_node *BTreeNode;

/** The type alias `btree_visitor_fp' refers to a function pointer that
 * visits a `btree_node' by reference. */
typedef void (*btree_visitor_fp)(BTreeNode);

/** The type alias `btree_pred_fp' refers to a function pointer that
 * acts as a _predicate_: it visits a node and reports whether that
 * node's item matches a particular condition. */
typedef bool (*btree_pred_fp)(Item);

/** Types of traversals possible on a binary search tree. */
typedef enum btree_traversal {
	BTREE_TRAVERSE_PREFIX,      /**< Order: node, left, right */
	BTREE_TRAVERSE_INFIX,       /**< Order: left, node, right */
	BTREE_TRAVERSE_POSTFIX,     /**< Order: left, right, node */
	BTREE_TRAVERSE_LEVEL,       /**< Order: left-to-right by level */
} btree_traversal;

/** Get the value at the current binary-tree node. */
Item btree_node_value (BTreeNode tree);

/**
 * Insert a value into a binary tree, maintaining search-tree ordering;
 * duplicates are inserted to the left of the tree.
 *
 * @param tree	the btree to insert into
 * @param value	the value to insert
 * @returns the root of the resulting tree
 */
BTreeNode btree_insert (BTreeNode tree, Item value);

/**
 * Search for a value in a binary tree with search-tree ordering.
 *
 * @param tree	the btree to search in
 * @param key	the value to search for
 * @returns the node that matches the key, or `NULL' if not found.
 */
BTreeNode btree_search (BTreeNode tree, Item key);

/**
 * Search for, and delete, a value in a binary tree, maintaining the
 * search-tree ordering.  Promote the leftmost node of the right subtree
 * on deletion where necessary.
 *
 * @param tree	the btree to search in and delete from
 * @param key	the key to search and delete
 * @returns the root of the resulting tree
 */
BTreeNode btree_delete_node (BTreeNode tree, Item key);

/**
 * Perform a traversal of the tree.
 *
 * @param tree	the btree to traverse
 * @param how	the ordering to use when traversing
 * @param visit	the function pointer to use when traversing,
 *              or NULL to produce an array of nodes.
 * @returns NULL if traversing with a function pointer,
 *          or a dynamically allocated array of nodes.
 */
BTreeNode *btree_traverse (
	BTreeNode tree, btree_traversal how, btree_visitor_fp visit);

/** Returns the number of nodes in the tree. */
size_t btree_size (BTreeNode tree);

/** Returns the number of leaf nodes in the tree. */
size_t btree_size_leaf (BTreeNode tree);

/** Returns the height of a tree. */
size_t btree_height (BTreeNode tree);

/** Destroy a tree, releasing all resources it requires. */
void btree_drop (BTreeNode tree);

/**
 * Return the number of nodes which match the predicate.
 *
 * @param tree	the btree to traverse
 * @param pred	a function pointer to the predicate to match
 */
size_t btree_count_if (BTreeNode tree, btree_pred_fp pred);

/** Is an item even? */
bool even_p (Item it);

/** Is an item odd? */
bool odd_p (Item it);

/** Is an item negative? */
bool negative_p (Item it);

#endif // !defined(CS2521__BTREE_H_)
