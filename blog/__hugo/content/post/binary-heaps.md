---
title: "Binary Heaps"
date: 2018-12-04T14:40:39+11:00
categories: ["Posts"]
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

In addition, binary heaps have a 'complete tree' property, that each level is as filled much as possible.

## Building a (Max) Heap
ie {3, 1, 6, 5, 2, 4}
(Children less than or equal to parent)

Go top-down, left-to-right. Swap nodes if child > parent


## Heap - Array Implementation
A heap can be represented as an array

The left child of a node (i) is at position `2i`  
The right child of a node (i) is at position `2i+1`  
The parent of a node (i) is `i/2`  

```c
#define parentOf(k) k/2
#define leftChildOf(k) k*2
#define rightChildOf(k) k*2
```

```c
// move value at a[k] to correct position
void heap_fixup (Item a[], size_t k) {
  while (k > 1 && item_cmp (a[k/2], a[k]) < 0) {
    swap (a, k, k/2);
    k /= 2;
  }
}
```

```c
// move value at a[k] to correct position
void heap_fixdown (Item a[], size_t k) {
  while (2 * k <= N) {
    size_t j = 2 * k; // choose greater child
    if (j < N && item_cmp (a[j], a[j+1]) < 0) j++;
    if (item_cmp (a[k], a[j]) >= 0) break;
    swap (a, k, j);
    k = j;
  }
}
```

### Properties
Height: `log n`  
Insert: `log n`  
Delete: `log n`  

### Example
{H, E, A, P, S, F, U, N}

```
     U    
   P   S  
  N H A F 
 E        
```