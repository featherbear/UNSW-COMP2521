---
title: "Binary Trees"
date: 2018-12-04T14:40:39+11:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

In programming, a tree is an efficient system of storing data.  
It is useful for representing hierarchical data, and to perform efficient searches

Trees are branched data stractures, consisting of nodes and edges, with no cycles (loops).
Each node contains a value.
Each node has edges to <= k other nodes.

--> And tree nodes are **unique** (treat them as keys)

# Tree Terminology
Parent - If has outgoing edges  
Child - If has incoming edges  
Root - If no parents (very top)  
Leaf - If no children (very bottom)  
Depth - Number of level (0-based)  

Balanced Tree - Tree with the smallest possible depth  
Degenerate Tree - Tree with the largest possible depth  
  
# Binary Trees
> COMP2521 focuses on Binary Trees

Binary Trees are trees which have, at most, two subtrees (where `k = 2`).  

Values in the left subtree are less than the node's value.  
Values in the right subtree are more than than node's value.  

degenerate - as most n-1 nodes  
balanced - at least log2 n  

### Cost for insertion
Balance O(log2 n), degenerate O(n)

### Cost for search/deletion
Balanced O(log2 n), degenerate O(n)

This is what a binary tree would look like, with the values `{4, 2, 6, 5, 1, 7, 3}` 
![Binary Tree](/post/binaryTree.gif)

## Serialisation of a Binary Tree
_Serialisation_ is the process of flattening a structure.  
(_Consider turning an object into JSON_)

There are different approaches...

# Depth-first
> 
## Pre-Order Traversal (NLR)
> Node, Left, Right  

4, 2, 1, 3, 6, 5, 7
## In-Order Traversal (LNR)
> Left, Node, Right  

1, 2, 3, 4, 5, 6, 7
## Post-Order Traversal (LRN)
> Left, Right, Node  

1, 3, 2, 5, 7, 6, 4

# Breadth-first
## Level-Order Transversal
> Left to Right  

4, 2, 6, 1, 3, 5, 7

# BinaryTree.c

Consider a Binary Tree structure implemented by
```c
typedef struct btree_node btree_node;

struct btree_node {
	Item item;
	btree_node *left;
	btree_node *right;
};
```

## Sum the items
> Return current plus childrens

```c
int int_btree_sum (btree_node *tree) {
   if (tree == NULL) return 0; // If we hit an empty node, return 0
   return tree->item           // Return the current node's value plus the values of its children nodes
          + int_btree_sum(tree->left) + int_btree_sum(tree->right);
}
```

## Search for an item
> Check current, else check children

```c
bool btree_search (btree-node *tree, Item key) {
    if (tree == NULL) return false;

    Item cmp = tree->item;
    if (cmp == key) return true;
    if (cmp > key) return btree_search(tree->left, key);
    if (cmp < key) return btree_search(tree->right, key);

    return false;
}
```

## Insert an item into a tree
> Find location, create node, link parent

```c
btree_node *btree_insert(btree_node *root, int it) {  
  if (root == NULL) {
    root = malloc(sizeof(btree_node));
    (*root) = (btree_node) {
      .item = it,
      .left = NULL,
     .right = NULL
    };
  }

  else if (it < root->item) {
    root->left = btree_insert(root->left, it);
  } else if (it > root->item) {
    root->right = btree_insert(root->right, it);
  }

  // what if it's the same

  return root;
}
```

## Delete an item from a tree
Deletion is much harder than insertion!  
> Find node, unlink, delete, **relink other nodes**

If we delete a node with children, we need to promote the children. If there are two children, the **leftmost of the right subtree** should be promoted.

## Drop a tree
```c
void btree_drop (btree_node *tree) {
  if (tree == NULL) return;

  btree_drop(tree->left);
  btree_drop(tree->right);
  free(tree);
}
```
