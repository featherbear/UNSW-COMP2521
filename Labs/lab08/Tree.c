////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... lab 8
//
// An implementation of a BST with a field (size) to keep how many nodes
// are in each sub-tree.  This implementation also includes functions to
// balance the tree

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <time.h>

#include "Tree.h"
#include "Item.h"

// The maximum number of levels that tree_display will print out.
#define MAX_PRINT_LEVELS 15

typedef struct tree tree;
typedef struct tree_node tree_node;

struct tree {
	struct tree_node {
		Item item;
		size_t size;
		tree_node *left, *right;
	} *root;
	enum tree_strategy strategy;
};

// Creation / Deletion functions.
static tree_node *tree_node_new (Item, tree_node *, tree_node *, size_t);
static void destroy_nodes (tree_node *n);

// Search functions
static bool search_normal (tree_node *t, Key k);
static tree_node *search_splay (tree_node *n, Key k, bool *found);

// Insert functions
static tree_node *insert_normal (tree_node *curr, Item item);
static tree_node *insert_at_root (tree_node *curr, Item item);
static tree_node *insert_random (tree_node *curr, Item item);
static tree_node *insert_splay (tree_node *tree, Item item);

// Balancing functions
static tree_node *rotate_left (tree_node *t);
static tree_node *rotate_right (tree_node *t);
static tree_node *balance (tree_node *tree);
static tree_node *partition (tree_node *t, size_t pos);

// Display and info functions
static ssize_t tree_height_recursive (tree_node *n);
static void dump (tree_node *n, size_t level, size_t max_level);

static void unimplemented (void) __attribute__((noreturn));

// If we were to just do `(a > b ? a : b)',
// we would evaluate one term at least twice.
// This is the _correct_ way to work around it.
#define max(a,b) \
	({ \
		typeof (a) _a = (a); \
		typeof (b) _b = (b); \
		_a > _b ? _a : _b; \
	})

// Initialises a tree.
//
// In this implementation, a tree is not just a pointer to the root node
// but is a struct which contains a pointer to the root node, as well as
// the balancing strategy used.
tree *tree_new (enum tree_strategy balance_strategy)
{
	tree *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate tree");
	*new = (tree) { .root = NULL, .strategy = balance_strategy };

	// seed random number generator for insert_random
	if (new->strategy == RANDOMISED)
		srand ((unsigned int) time (NULL));

	return new;
}


// TODO: YOU NEED TO MODIFY THIS FOR TASK 2
//
// Insert an item into a binary tree and balance the tree,
// depending on the defined rebalance strategy.
//
// We will assume we do not care about duplicates for this
void tree_insert (tree *tree, Item it)
{
	assert (tree != NULL);

	switch (tree->strategy) {
	case NO_REBALANCE:
		tree->root = insert_normal (tree->root, it);
		break;

	default:
		printf ("STRATEGY NOT IMPLEMENTED\n");
		exit (1);
	}
}

// Count the number of items in the tree.
size_t tree_count (tree *tree)
{
	if (tree->root == NULL) return 0;
	return tree->root->size;
}

// TODO: You must change this for the bonus challenge task
//
// Search the tree for a specified key.
// Uses normal search or splay search, depending on
// the balance strategy
bool tree_search (tree *tree, Key k)
{
	assert (tree != NULL);
	switch (tree->strategy) {
	case SPLAY:
		unimplemented ();
	default:
		return search_normal (tree->root, k);
	}

	__builtin_unreachable();
}

// Calculate the height of the tree.
ssize_t tree_height (tree *tree)
{
	assert (tree != NULL);
	return tree_height_recursive (tree->root);
}

// Free the memory associated with the tree.
void tree_destroy (tree *tree)
{
	assert (tree != NULL);
	destroy_nodes (tree->root);
	free (tree);
}

// Display the tree.
void tree_display (tree *tree)
{
	assert (tree != NULL);
	dump (tree->root, 0, MAX_PRINT_LEVELS);
}

// Print out some statistics about the tree.
void tree_print_stats (tree *tree)
{
	switch (tree->strategy) {
	case NO_REBALANCE:
		puts ("Strategy: none"); break;
	case REBALANCE_1:
		puts ("Strategy: Global rebalance, 1"); break;
	case REBALANCE_100:
		puts ("Strategy: Global rebalance, 100"); break;
	case REBALANCE_1000:
		puts ("Strategy: Global rebalance, 1000"); break;
	case RANDOMISED:
		puts ("Strategy: Randomised"); break;
	case SPLAY:
		puts ("Strategy: Splay"); break;
	}

	printf (
		"#nodes = %zu, height = %zu\n",
		tree_count (tree),
		tree_height (tree)
	);
}

////////////////////////////////////////////////////////////////////////
// Tree creation / deletion functions

// Creates a new tree node, with the specified item, left and right
// child nodes, and size.
static tree_node *tree_node_new (
	Item item, tree_node *l, tree_node *r, size_t size)
{
	tree_node *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate tree_node");
	*new = (tree_node) {
		.item = item,
		.left = l,
		.right = r,
		.size = size,
	};
	return new;
}

// Destroys all of the nodes associated with a tree.
static void destroy_nodes (tree_node *n)
{
	if (n == NULL) return;
	destroy_nodes (n->left);
	destroy_nodes (n->right);
	free (n->item);
	free (n);
}


////////////////////////////////////////////////////////////////////////
// Tree insertion functions

// A recursive version of standard BST insertion
// Modified to update the size counters
static tree_node *insert_normal (tree_node *curr, Item item)
{
	if (curr == NULL)
		return (tree_node_new (item, NULL, NULL, 1));

	if (less (key (item), key (curr->item)))
		curr->left = insert_normal (curr->left, item);
	else
		curr->right = insert_normal (curr->right, item);

	curr->size++;
	return curr;
}

// Randomly inserts either at the leaf of the tree or the root.
static tree_node *insert_random (tree_node *curr, Item item)
{
	if (curr == NULL)
		return (tree_node_new (item, NULL, NULL, 1));

	// Inserts at root with probability 1/N
	double i = (curr->size + 1);
	if (rand () < (RAND_MAX / i))
		return insert_at_root (curr, item);
	else if (less (key (item), key (curr->item)))
		curr->left = insert_random (curr->left, item);
	else
		curr->right = insert_random (curr->right, item);

	curr->size++;
	return curr;
}


// A recursive function that inserts the new item
// at the root of the tree. This is used in insert_random.
static tree_node *insert_at_root (tree_node *curr, Item item)
{
	if (curr == NULL)
		return (tree_node_new (item, NULL, NULL, 1));

	// Note: the rotate functions are responsible for updating the size.
	if (less (key (item), key (curr->item))) {
		curr->left = insert_at_root (curr->left, item);
		curr = rotate_right (curr);
	} else {
		curr->right = insert_at_root (curr->right, item);
		curr = rotate_left (curr);
	}

	return curr;
}


////////////////////////////////////////////////////////////////////////
// Tree search functions

// Performs a standard recursive search for the key in the given tree
// returns 1 if found and 0 otherwise
bool search_normal (tree_node *t, Key k)
{
	if (t == NULL)
		return false;
	if (less (k, t->item))
		return search_normal (t->left, k);
	if (greater (k, t->item))
		return search_normal (t->right, k);
	return true;
}

////////////////////////////////////////////////////////////////////////
// Tree rotation functions

// This function does not update size.
// TODO: YOU MUST FIX THIS
static tree_node *rotate_left (tree_node *curr)
{
	if (curr == NULL || curr->right == NULL)
		return curr;

	tree_node *rotated_left = curr->right;
	curr->right = rotated_left->left;
	rotated_left->left = curr;
	return rotated_left;
}

// This function does not update size.
// TODO: YOU MUST FIX THIS
static tree_node *rotate_right (tree_node *curr)
{
	if (curr == NULL || curr->left == NULL)
		return curr;

	tree_node *rotated_right = curr->left;
	curr->left = rotated_right->right;
	rotated_right->right = curr;
	return rotated_right;
}

////////////////////////////////////////////////////////////////////////
// Tree balancing functions

static tree_node *balance (tree_node *tree)
{
	if (tree->size < 2) return tree;

	tree = partition (tree, tree->size / 2);
	tree->left = balance (tree->left);
	tree->right = balance (tree->right);
	return tree;
}

// partition tree at node with position pos (counting from 0) in the
// sorted sequence of all items, node become new root node.
static tree_node *partition (tree_node *curr, size_t pos)
{
	if (curr == NULL) return curr;

	size_t left_subtree_size = curr->left->size;
	if (left_subtree_size > pos) {
		curr->left = partition (curr->left, pos);
		curr = rotate_right (curr);
	} else if (left_subtree_size < pos) {
		curr->right = partition (curr->right, pos - 1 - left_subtree_size);
		curr = rotate_left (curr);
	}
	return curr;
}


////////////////////////////////////////////////////////////////////////
// Splay tree functions

static tree_node *insert_splay (tree_node *tree, Item item)
{
	if (tree == NULL)
		return (tree_node_new (item, NULL, NULL, 1));

	if (less (key (item), key (tree->item))) {
		if (tree->left == NULL)
			return (tree_node_new (item, NULL, tree, tree->size + 1));

		if (less (key(item), key (tree->left->item))) {
			tree->left->left = insert_splay (tree->left->left, item);
			tree = rotate_right (tree);
		} else {
			tree->left->right = insert_splay (tree->left->right, item);
			tree->left = rotate_left (tree->left);
		}

		return rotate_right (tree);

	} else {
		if (tree->right == NULL)
			return (tree_node_new (item, tree, NULL, tree->size + 1));

		if (less (key (tree->right->item), key(item))) {
			tree->right->right = insert_splay (tree->right->right, item);
			tree = rotate_left (tree);
		} else {
			tree->right->left = insert_splay (tree->right->left, item);
			tree->right = rotate_right (tree->right);
		}

		return rotate_left (tree);
	}
}

// The function should perform rotations on all
// items in the search path in a similar way to that of
// splay tree insertion - except no node is actually inserted

// The node that contains the key
// should be rotated up and returned as the root of the tree
// and *found should be set to 1

// If the key was not found, the last node on the search
// path should be rotated up to the root of the tree
// and found should be set to 0

// returns the new root of the tree
// sets the value of *found to 0 or 1
static tree_node *search_splay (tree_node *n, Key k, bool *found)
{
	tree_node *return_val = NULL;

	if (n == NULL) {
		// item not found
		*found = false;
		return_val = n;
	} else if (eq (key (n->item), k)) {
		*found = true; // item found, store true
		return_val = n;
	} // else if (...) {

	// TODO: COMPLETE THE IMPLEMENTATION HERE
	// The function should perform rotations on all
	// items in the search path in a similar way to that of
	// splay tree insertion - except no node is actually inserted

	// The node that is found
	// should be returned as the root of the tree
	// and *found should be set to 1
	// If the node was not found, the last node on the search
	// path should be rotated up to the root of the tree
	// and found should be set to 0

	return return_val;
}


// Tree height functions
// ---------------------------------------------------------------------

// Finds the height of a tree with root n
static ssize_t tree_height_recursive(tree_node *n)
{
	if (n == NULL) return -1;
	return 1 + max (
		tree_height_recursive (n->left),
		tree_height_recursive (n->right)
	);
}


// Tree display functions
// ---------------------------------------------------------------------

// Prints out a tree
// Handles recursion and level maintenance
static void dump (tree_node *n, size_t level, size_t max_level)
{
	if (n == NULL || n == NULL) {
		printf ("X\n");
	} else if (level == max_level) {
		printf ("...\n");
	} else {
		item_show (n->item);
		printf (" (%zu) ", n->size);
		printf ("\n");
		if (n->left != NULL) {
			for (size_t i = 0; i < level; i++)
				printf ("|  ");
			printf ("|--L: ");
			dump (n->left, level + 1, max_level);
		}
		if (n->right != NULL) {
			for (size_t i = 0; i < level; i++)
				printf ("|  ");
			printf ("|--R: ");
			dump (n->right, level + 1, max_level);
		}
	}
}

static void unimplemented (void)
{
	puts ("Not implemented.");
	exit (EXIT_FAILURE);
}
