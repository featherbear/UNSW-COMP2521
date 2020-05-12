typedef struct btree_node btree_node;
struct btree_node {
	Item item;
	btree_node *left, *right;
};

////////////////////////////////////////////////////////////////////////

btree_node *btree_insert (btree_node *tree, Item value)
{
	if (tree == NULL) return btree_node_new (value);
	int cmp = item_cmp (tree->item, value);
	if (cmp <  0) {
		tree->right = btree_insert (tree->right, value);
	} else if (cmp  > 0) {
		tree->left  = btree_insert (tree->left, value);
	} else { /* cmp == 0, do nothing */ }
	return tree;
}

////////////////////////////////////////////////////////////////////////

/** returns the number of nodes in the tree */
size_t btree_size (btree_node *tree)
{
	if (tree == NULL) return 0;
	return 1
		+  btree_size (tree->left)
		+  btree_size (tree->right);
}

/** returns the height of a tree */
size_t btree_height (btree_node *tree)
{
	if (tree == NULL) return 0;
	size_t lheight = btree_height (tree->left);
	size_t rheight = btree_height (tree->right);
	return 1
		+  (lheight > rheight) ? lheight : rheight;
}

////////////////////////////////////////////////////////////////////////

// N, L, R
void btree_traverse_prefix (btree_node *tree)
{
	if (tree == NULL) return;
	btree_traverse_visit (tree);
	btree_traverse_prefix (tree->left);
	btree_traverse_prefix (tree->right);
}

// L, N, R
void btree_traverse_infix (btree_node *tree)
{
	if (tree == NULL) return;
	btree_traverse_infix (tree->left);
	btree_traverse_visit (tree);
	btree_traverse_infix (tree->right);
}

// L, R, N
void btree_traverse_postfix (btree_node *tree)
{
	if (tree == NULL) return;
	btree_traverse_postfix (tree->left);
	btree_traverse_postfix (tree->right);
	btree_traverse_visit (tree);
}

void btree_traverse_level (btree_node *tree)
{
	// hint #1: don't recurse
	// hint #2: consider previous data structures
}

void btree_traverse_visit (btree_node *tree)
{
	char *s = item_show (tree->item);
	puts (s);
	free (s);
}

////////////////////////////////////////////////////////////////////////

btree_node *
btree_delete_node (btree_node *tree, Item item)
{
	if (tree == NULL) return NULL;
	int cmp = item_cmp (tree->item, item);
	if (cmp == 0) {
		// tree has no subtrees:
		if (tree->left == NULL && tree->right == NULL)
			/* drop it */;
		else if ((tree->left != NULL && tree->right == NULL) ||
				 (tree->right != NULL && tree->left == NULL))
			/* promote the subtree that exists */;
		else {
			// node := leftmost of right (iteratively)
			// promote node -- update left, right
			// btree_node_drop (tree)
		}
	} else if (cmp > 0) {
		tree->left  = btree_delete_node (tree->left,  item);
	} else if (cmp < 0) {
		tree->right = btree_delete_node (tree->right, item);
	}
	
	return tree;
}
