---
title: "Tutorial 4 - Heaps and Graphs"
date: 2019-01-03T14:10:18+11:00
categories: ["Tutorial / Labs"]

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

# Heaps
Heaps are a form of data representation, often for priority queues - where the max (or min) value is at the top of the tree

## Representation
```
   1 
 3   5
7 6 9 8
```

`Items: [X, 1, 3, 5, 7, 6, 9, 8]`  
`Index:  0  1  2  3  4  5  6  7`

## Properties
* **Heap ordering property** - Node is always bigger (or always smaller) than children  
* **Complete tree property** - Filled as much as possible (minimum possible depth)

## Deleting a node
Switch the node with its bottom-most right-most value, and perform a fix-down

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

# Graphs

* **path** - A to B
* **cycle** - A to B to A
* **spanning tree** - all vertices only have one connection
* **clique** - all vertices are connected to each other

## Graph ADT
* **Adjacency Matrix** - An array (size n) of an array (size n)
* **Adjacency List** - An array (size n) of linked lists

### Example
![](Snipaste_2019-01-03_14-36-21.png)

#### Adjacency Matrix  
|||||||
|:--:|:--:|:--:|:--:|:--:|:--:|
|0|1|1|0|0|0|
|1|0|0|1|0|0|
|1|0|0|1|1|0|
|0|1|1|0|0|0|
|0|0|1|0|0|1|
|0|0|0|0|1|0|

Connected if `(g[i][j] == 1)`  

#### Adjacency List
```
0: 1 -> 2 -> NULL
1: 0 -> 3 -> NULL
2: 0 -> 3 -> 4 -> NULL
3: 1-> 2 -> NULL
4: 2 -> 5 -> NULL
5: 4 -> NULL
```

Connected if `for (adjnode v = g->n[i]; v; v = v->next) if (v.item == j) return true;)); return false`

### Counting Edges
Find the number of `true` items in the adjacency list, and then divide by two

If using an adjacency list (with results `(a,b)`), count while `a < b`

Matrix - dense  
List - sparse  


## Adjacency Matrix Optimisation
Since an adjacency matrix is mirrored diagonally (for simple graphs) - we are using unecessary memory, so we should cut down!

![](Snipaste_2019-01-03_14-54-44.png)  

`v . nV - (v(v+1)/2)`
