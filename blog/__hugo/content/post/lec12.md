---
title: "Lecture 12 - Balanced Trees"
date: 2019-01-22T14:42:56+11:00
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

* A tree is weight balanced if: (size(l) - height\(r)| < 2  
* A tree is height balanced if: |height(l) - height\(r)| < 2  

# Tree Rotation
Operations that change the balance of a tree whilst maintaining a search tree

## Left Rotate
left subtree moves down  
right subtree promoted  
left child of right subtree becomes right child of left subtree  

```c
btree_node *btree_rotate_left(btree_node *n1) {
  if (!n1) return NULL;

  btree_node *n2 = n1->right;
  if (!n2) return n1;

  n1->right = n2->left;
  n2->left = n1;

  return n2;
}

```


## Right Rotate
right subtree moves down  
left subtree promoted  
right child of left subtree becomes left child of right subtree  

```
  B    ->    A
A   C  ->  C   B
```

```c
btree_node *btree_rotate_right(btree_node *n1) {
  if (!n1) return NULL;

  btree_node *n2 = n1->left;
  if (!n2) return n1;

  n1->left = n2->right;
  n2->right = n1;

  return n2;
}
```

**Rebalance cost**  
Many O(n)  
Degenerate O(n logn)  

**When to rebalance**  
Rebalance on every inseration  
or rebalance every k values  
or rebalance when a threshold is passed  

// Amortisation - more work now less work later

