---
title: "Lecture 5 - Priority Queues and Graphs"
date: 2018-12-11T14:45:10+11:00

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

# Binary Heaps
A binary heap is similar to a binary search tree. They differ in the way which data is stored.

A binary heap guarantees top-down maxmimum to minimum.  
A binary search tree guarantees left-to-right maximum to minimum

In addiion, binary heaps have a 'complete tree' property, that each level is as filled much as possible.

## Building a (Max) Heap
ie {3, 1, 6, 5, 2, 4}
(Children less than or equal to parent)

Go top-down, left-to-right. Swap nodes if child > parent


## Heap - Array Implementation
A heap can be represented as an array

The left child of a node (i) is at position `2i`  
The right child of a node (i) is at position `2i+1`  
The parent of a node (i) is `i/2`  











# Priority Queues
## What is a priority queue

### Time Complexity
## Unordered
Insert: O(1)
Delete: O(n)

## Ordered
Insert: O(n)
Delete: O(1)

# Binary [Search] Trees
Subtrees of a node will be less than a node



# Heaps
Heaps are **complete trees**.  
_complete tree_: Where every level is filled

An array can be used.

0 1 2 3 4 5 6
  t q k m a h
left(i) = 2i
right(i) = 2i + 1
parent(i) = i/2

## Insertion 
1) Add new elements at the bottom-most right position
2) (but now it's not a heap, sooo) fix up

--> fixup
```c
void heap_fixup(Item a[], size_t k) {
  while (k > 1 && item_cmp(a[k/2], a[k] > 0)) {
    --
  }
}

### Exercise
