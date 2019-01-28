---
title: "Lecture Twelve - Balanced Trees"
date: 2019-01-22T14:42:56+11:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

Weight balance (size(l) - height(r)| < 2
Height balance |height(l) - height(r)| < 2

Left Rotation
Right Rotation

A pair of 'primitive' operations that change the blance of a tree whilst maintaining a search tree

Right rotation (rotate right)
right subtree moves down
left subtree promoted
right child of left subtree becomes left child of right subtree
  B       ->   A
A   C     -> C   B


```c
btree_node *btree_rotate_right(btree_node *n1) {
  if (!n1) return NULL;

  btree_node *n2 = n1->left;
  if (!n2) return n1;

  n1->left = n2->right;
  n2->right = n1;

  return n2;
}

// Keep a record of n2
// Reassign n1->left to n2->right
// Reassign n2->right to n1


```

```c


/*
    A
  B   C   

*/

btree_node *btree_rotate_left(btree_node *n1) {
  if (!n1) return NULL;

  btree_node *n2 = n1->right;
  if (!n2) return n1;

  n1->right = n2->left;
  n2->left = n1;

  return n2;
}

```





Rebalance cost 
Many O(n)
Degenerate O(n logn)

Insert? 
Rebalance on every inseration
or rebalance every k values
Or rebalance when a threshold is passed

amortisation

