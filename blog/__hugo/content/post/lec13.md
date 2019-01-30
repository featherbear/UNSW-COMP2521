---
title: "Lecture 13 - Splay Trees, 2-3-4 Trees, Redblack Trees"
date: 2019-01-24T10:27:48+11:00
categories: ["Lectures"]

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

# Splay Trees

Amortisation is the process of decreasing the average cost of an operation.  
Though it might take more computation time to perform the amortisation, the results are favourable

LL -> RR  
LR -> LR  
RR -> LL  
RL -> RL  

```c
btree_node *btree_rotate_left(btree_node *);
btree_node *btree_rotate_right(btree_node *);

btree_node *btree_insert_splay(btree_node *tree, Item it);
btree_node *btree_search_splay(btree_node **root, Item it);
```

## Inserting into a Splay Tree
```c
btree_node *btree_insert_splay(btree_node *tree, Item it) {
  if (!tree) return btree_node_new(it, NULL, NULL);
  int cmp = item_cmp(it, tree->value);

  if (cmp < 0) {
    if (!tree->left) {
      tree->left = btree_node_new(it, NULL, NULL);
      return tree;
    }

    int ldiff = item_cmp(it, tree->left->value);
    if (ldiff < 0) {
      // left left
        tree->left->left = btree_insert_splay(tree->left->left);
        tree->left = btree_rotate_right(tree->left);
    } else {
        tree->left->right = btree_insert_splay(tree->left->right);
        tree->left = btree_rotate_left(tree->left);
      // left right
    }

    tree = btree_rotate_right(tree);
    return tree;
  }

  if (cmp > 0) {
    if (!tree->right) {
      tree->right = btree_node_new(it, NULL, NULL);
      return tree;
    }

    int rdiff = item_cmp(it, tree->right->value);
    if (rdiff < 0) {
      // right left
        tree->right->left = btree_insert_splay(tree->right->left);
        tree->right = btree_rotate_left(tree->right);
    } else {
      // right right
      tree->right->right = btree_insert_splay(tree->right->right);
      tree->right = btree_rotate_right(tree->right);

    }
    tree->left = btree_insert_splay(tree->left);
  }
  else tree->value = it;
}
```

## Searching a Splay Tree
```c
// Searching is now optimised
btree_node *btree_search_splay(btree_node **root, Item it) {
  assert(tree);
  if (!*root) return NULL;
  *root = btree_splay(*root, it);
  if (item_cmp((*root)->value) == 0) return *root;
  return NULL;
}
```

---

Average O((N+M) log(N+M))
N nodes
M inserts/searches

# 2-3-4 Trees
2-nodes - one node, two children  
3-nodes - two value, three chldren  
4-nodes - three values, four children  

Depth O(logn)  
Worst case - all 2 nodes  
Best case - all 4 nodes  

**Insertion**  
- find leaf node (search)  
- check if not full; then insert   
  - else split two 2nodes  
  - promote middle to parent  
  - if parent is a 4node, split/promote again  

# Redblack Trees
Redblack trees are a method of representing a 2-3-4 tree with standard BST nodes, with the addition of adding extra meta info into the BST `struct`ure.

Uses plain BST nodes (just with extra meta || encoded link type)  
Uses 2-3-4 rules  

black links - normal  
red links - sibling  

No two red on a path  
All paths need the same number of black links  

painful. :(