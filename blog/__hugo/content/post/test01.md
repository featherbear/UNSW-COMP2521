---
title: "Class Exam 1"
date: 2019-01-10T12:00:00+11:00
description: "Questions and solutions to the 19T0 first class exam"
categories: ["Assessments"]

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

Had my first practice exam for COMP2521 this week!.

# Sorted Array Validation
**Task**  
Write a function that recursively checks if an array is sorted  

**Prototype**  
`bool search_p (int xs[], int l, int r)`  
`xs` - array  
`l` - left index  
`r` - right index

**Code**
```c
bool search_p (int xs[], int l, int r) {
    // exit-case: if the left and right indices cross
    if (l > r) return true;

    // check the indices
    if (xs[l] > xs[r]) return false;

    return search_p(xs, l+1, r);
}
```

# Fibonacci Linked List

> Uhm I lost 0.1 marks from this question...

**Task**  
Write a function that generates a linked list of `n` Fibonacci numbers.
_This question had a quirk... fib(0) -> NULL, and fib(1) -> item(0)_

**Prototype**  
`node *fibonacci(int n)`

**Code**  
```c
typedef struct node *Node;
struct node {
    int item;
    Node next;
};

/*
 *  Function: new_node
 *  Creates a node with the item value `i`
 */
Node new_node(int i) {
    Node new = malloc(sizeof(*new));
    (*new) = (struct node) {
        .item = i,
        .next = NULL
    }
}

node *fibonacci(int n) {
    if (n == 0) return NULL;

    Node head = new_node(0);  // Set up the 1st element (0)
    if (n == 1) return head;  // And return if fibonacci(1)

    Node tail = new_node(1);  // Set up the 1st element (0)
    head->next = tail;
    if (n == 2) return head;  // And return if fibonacci(2)

    //

    // Variables to hold the last two values
    int prev1 = head->item; // 0
    int prev2 = tail->item; // 1

    for (int i = 3; i <= n; i++) {
        // Make a new node out of the sum of the last two values
        int nextVal = prev1 + prev2;
        Node n = new_node(nextVal);
        
        // Link and reassign the tail
        tail->next = n;
        tail = n;

        // Update the last two values
        prev1 = prev2;
        prev2 = n->item;
    }

    return head;
}
```

# DList Relink
**Task**  
Given two doubly linked lists `x` and `y`, write a function that moves all elements from `y` to `x`

**Prototype**  
`void list_relink(DList x, DList y)`

**Code**  
```c
typedef struct dlist *DList;
typedef struct dnode *DNode;

struct dlist {
    int size;
    DNode head;
    DNode tail;
};


struct dnode {
    int item;
    DNode next;
    DNode tail;
};

//

void list_relink(DList x, DList y) {
    // If y is empty then we don't need to do anything
    if (y->size == 0) return;

    if (x->size == 0) {
        // If x is empty, then just move the head from y
        x->head = y->head;
    } else {
        x->tail->next = y->head;
        y->head->prev = x->tail;
    }
    
    // Update the new meta for x
    x->tail = y->tail;
    x->size += y->size;

    // Unlink y
    y->head = NULL;
    y->tail = NULL;   
    y->size = 0;
}
```

# BST Validation
**Task**  
Given a binary search tree, write a function to check that the search property is held true.

**Prototype**  
`bool bst_valid(node *tree)`

**Code**  
```c
typedef struct node *Node;
typedef struct node {
    int item;
    Node left;
    Node right;
}

bool bst_valid(Node tree) {
    if (tree == NULL) return true;

    // if the left subtree exists, ensure that its item is less than the parent
    if (tree->left) {
        if (!(tree->left->item < tree->item)) return false;
    }

    // if the right subtree exists, ensure that its item is more than the parent
    if (tree->right) {
        if (!(tree->right->item > tree->item)) return false;
    }

    // check down the tree
    return bst_valid(tree->left) && bst_valid(tree->right);
}
```

# BST Prune

> I lost 3 marks from this question, not sure where since they don't release the solutions

**Task**  
Write a function that drops the nodes past a given depth

**Prototype**  
`void btree_prune(Node tree, int depth)`

**Code**  
```c
typedef struct node *Node;
typedef struct node {
    int item;
    Node left;
    Node right;
}

/*
 * Function: btree_drop
 * Recursively release all nodes
 */
void btree_drop(Node tree) {
    if (!tree) return;
    btree_drop(tree->left);
    btree_drop(tree->right;
    free(tree);
}

//
Node btree_prune(Node tree, int depth) {
    if (depth == 0) {
        btree_drop(tree->left);
        btree_drop(tree->right);
        tree->left = tree->right = NULL;
        return NULL;
    } else {
        tree->left = btree_prune(tree->left, depth-1);
        tree->right = btree_prune(tree->right, depth-1);
    }
    return tree;
}
```

**Explanation**
In order to drop elements only greater than (or equal) to a given depth, I would need to implement some sort of counter.  
However, I could not modify the `btree_prune` function prototype. I **could** have written a helper function, and turn `btree_prune` into a wrapper function - but another was was to use the `depth` parameter!  

By decreasing `depth`, we are effectively saying _"`n` more levels to go until we start dropping everything"_.