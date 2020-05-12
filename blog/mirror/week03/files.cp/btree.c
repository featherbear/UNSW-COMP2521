////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- A binary search tree implementation.
//
// 2018-12-08	Jashank Jeremy <jashankj@cse.unsw.edu.au>
// YYYY-mm-dd	Your Name Here <zNNNNNNN@student.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "btree.h"
#include "item.h"
#include "item_int.h"
#include "testable.h"

typedef struct btree_node btree_node;
struct btree_node {
	Item item;
	btree_node *left, *right;
};

typedef struct traverse_state traverse_state;
struct traverse_state {
	btree_traversal how;
	size_t n_nodes, upto;
	btree_node **nodes;
	btree_visitor_fp visitor;
};

static btree_node *btree_node_new (Item it);

static void btree_traverse_prefix (btree_node *, traverse_state *);
static void btree_traverse_infix (btree_node *, traverse_state *);
static void btree_traverse_postfix (btree_node *, traverse_state *);
static void btree_traverse_level (btree_node *, traverse_state *);
static void btree_traverse_visit (btree_node *, traverse_state *);

// Remove `__unused' tags from your functions before flight.
#ifndef __unused
#define __unused __attribute__((unused))
#endif

////////////////////////////////////////////////////////////////////////

/** Get the value at the current binary-tree node. */
Item btree_node_value (btree_node *tree)
{
	if (tree == NULL)
		return NULL;
	else
		return tree->item;
}

/**
 * Insert a value into a binary tree, maintaining search-tree ordering;
 * duplicates are inserted to the left of the tree.
 *
 * @param tree	the btree to insert into
 * @param value	the value to insert
 * @returns the root of the resulting tree
 */
btree_node *btree_insert (btree_node *tree, Item value)
{
	if (tree == NULL)
		return btree_node_new (value);

	int cmp = item_cmp (tree->item, value);
	if (cmp <  0) {
		tree->right = btree_insert (tree->right, value);
	} else /* (cmp >= 0) */ {
		tree->left  = btree_insert (tree->left, value);
	}

	return tree;
}

/**
 * Search for a value in a binary tree with search-tree ordering.
 *
 * @param tree	the btree to search in
 * @param key	the value to search for
 * @returns the node that matches the key, or `NULL' if not found.
 */
btree_node *btree_search (btree_node *tree, Item item)
{
	if (tree == NULL) return NULL;
	int cmp = item_cmp (tree->item, item);
	if (cmp == 0) return tree;
	if (cmp <  0) return btree_search (tree->right, item);
	if (cmp  > 0) return btree_search (tree->left, item);
	return NULL;
}

/**
 * Search for, and delete, a value in a binary tree, maintaining the
 * search-tree ordering.  Promote the leftmost node of the right subtree
 * on deletion where necessary.
 *
 * @param tree	the btree to search in and delete from
 * @param key	the key to search and delete
 * @returns the root of the resulting tree
 */
btree_node *btree_delete_node (btree_node *tree, Item item __unused)
{
	warnx ("btree_delete_node unimplemented");
	// implement me!
	return tree;
}

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
btree_node **btree_traverse (
	btree_node *tree,
	btree_traversal how,
	btree_visitor_fp visit)
{
	traverse_state state;

	if (visit == NULL) {
		state.n_nodes = btree_size (tree);
		state.nodes = calloc (state.n_nodes, sizeof *state.nodes);
		if (state.nodes == NULL)
			err (EX_OSERR, "couldn't allocate nodes[]");
		state.upto = 0;
	}

	state.visitor = visit;
	state.how = how;

	switch (how) {
	case BTREE_TRAVERSE_PREFIX:
		btree_traverse_prefix (tree, &state);  break;
	case BTREE_TRAVERSE_INFIX:
		btree_traverse_infix (tree, &state);   break;
	case BTREE_TRAVERSE_POSTFIX:
		btree_traverse_postfix (tree, &state); break;
	case BTREE_TRAVERSE_LEVEL:
		btree_traverse_level (tree, &state);   break;
	}

	return state.nodes;
}

/** Returns the number of nodes in the tree. */
size_t btree_size (btree_node *tree)
{
	if (tree == NULL) return 0;
	return 1
		+  btree_size (tree->left)
		+  btree_size (tree->right);
}

/** Returns the number of leaf nodes in the tree. */
size_t btree_size_leaf (BTreeNode tree __unused)
{
	warnx ("btree_size_leaf unimplemented");
	// implement me!
	return 0;
}

/** Returns the height of a tree. */
size_t btree_height (btree_node *tree)
{
	if (tree == NULL) return 0;
	size_t lheight = btree_height (tree->left);
	size_t rheight = btree_height (tree->right);
	return 1
		+  ((lheight > rheight) ? lheight : rheight);
}

/** Destroy a tree, releasing all resources it requires. */
void btree_drop (btree_node *tree __unused)
{
	warnx ("btree_drop unimplemented");
	// implement me!
}

/**
 * Return the number of nodes which match the predicate.
 *
 * @param tree	the btree to traverse
 * @param pred	a function pointer to the predicate to match
 */
size_t btree_count_if (btree_node *tree, btree_pred_fp pred)
{
	warnx ("btree_count_if unimplemented");
	// implement me!
	return 0;
}

////////////////////////////////////////////////////////////////////////

static btree_node *btree_node_new (Item it)
{
	btree_node *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate btree_node");
	*new = (btree_node){ .item = it, .left = NULL, .right = NULL };
	return new;
}

/**
 * Perform a prefix-order (node, left, right) traversal.
 *
 * @param tree	the btree we're traversing in
 * @param state	the current state of the traversal
 */
static void btree_traverse_prefix (
	btree_node *tree,
	traverse_state *state)
{
	if (tree == NULL) return;
	btree_traverse_visit (tree, state);
	btree_traverse_prefix (tree->left, state);
	btree_traverse_prefix (tree->right, state);
}

/**
 * Perform an infix-order (left, node, right) traversal.
 *
 * @param tree	the btree we're traversing in
 * @param state	the current state of the traversal
 */
static void btree_traverse_infix (
	btree_node *tree,
	traverse_state *state)
{
	if (tree == NULL) return;
	btree_traverse_infix (tree->left, state);
	btree_traverse_visit (tree, state);
	btree_traverse_infix (tree->right, state);
}

/**
 * Perform a postfix-order (left, right, node) traversal.
 *
 * @param tree	the btree we're traversing in
 * @param state	the current state of the traversal
 */
static void btree_traverse_postfix (
	btree_node *tree,
	traverse_state *state)
{
	if (tree == NULL) return;
	btree_traverse_postfix (tree->left, state);
	btree_traverse_postfix (tree->right, state);
	btree_traverse_visit (tree, state);
}

/**
 * Perform a level-order traversal of the tree.
 * Call `btree_traverse_visit' to visit a node.
 *
 * @param tree	the btree we're traversing in
 * @param state	the current state of the traversal
 */
static void btree_traverse_level (
	btree_node *tree __unused,
	traverse_state *state __unused)
{
	warnx ("btree_traverse_level unimplemented");
	// implement me!
}

/**
 * Actually do the business of visiting one node.  If we are applying a
 * visitor function, apply it; if we are making an array, add this node.
 *
 * @param tree	the btree we're traversing in
 * @param state	the current state of the traversal
 */
static void btree_traverse_visit (
	btree_node *tree,
	traverse_state *state)
{
	assert (tree != NULL);

	if (state->visitor != NULL) {
		state->visitor (tree);
	} else {
		assert (state->upto <  state->n_nodes);
		state->nodes[state->upto++] = tree;
		assert (state->upto <= state->n_nodes);
	}
}


////////////////////////////////////////////////////////////////////////

void white_box_tests (void)
{
	warnx ("white_box_tests unimplemented");
	// implement me!
}
