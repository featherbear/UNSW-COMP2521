---
title: "Tutorial Five"
date: 2019-01-08T17:09:10+11:00
categories: ["Tutorial / Labs"]
description: "A look at more BFSs and DFSs"

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

# Traversal
*Pseudocode*  
```
Add item onto stack/queue

while stack/queue is not empty {
  dequeue/pop
  mark item as visited
  -> perform operations here if needed
  add neighbouring nodes
}
```

* DFS - Stack  
* BFS - Queue  


Find shortest (non-weighted) -> BFS (from a or b) (to b or a)

// DFS Search Example

// BFS Search Example

# Spanning Tree
## Kruskal&apos;s Algorithm
* Sort min max
* Add edges incrementally (as long as they don't lead to a cycle)
  * Add edge, if cycle created
  * Remove edge

## Prim&apos;s Algorithm
* Start at any
* Add neighbouring vertices



