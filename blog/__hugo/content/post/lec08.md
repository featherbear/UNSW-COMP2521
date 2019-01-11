---
title: "Lecture 8 - Directed and Weighted Graphs"
date: 2019-01-08T14:00:00+11:00
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

# Directed Graphs

* **undirected graphs** - an edge relates two vertices equivalently
* **directed graphs** - only one direction | self loops!?

## Keywords
* **transitive closure** - Directed path from **s** to **t**
* **shortest path search** - Shortest path from **s** to **t**
* **strong connectivity** - vertices mutually reachable

* **in-degree** (d^(-1)(v)) - number of directed edges leading into a vertex
* **out-degree** (d(v)) - number of directed edges leading out of a vertex

* **sink** - vertex with out-degree 0 (only in)
* **source** - vertex with in-degree 0 (only out)

* **reachability** - if v->w exists, w is reachable from v
* **strongly connected** - if v->w and w->v exists

* **strong connectivity** - every vertex reachable from every other vertex
* **strongly-connected component** - maximal strongly-connected subgraph

///////////

## Methods of representation
Adjacency matrix - asymmetric, spare, less space efficient  
Adjaceny list - !!!  
Edge list - order of edge components matter  
Linked data tructures - pointers inherently directional  

-|storage|edge add|has edge|out degree|
|:--:|:--:|:--:|:--:|:--:|
adj mat | O(V+V^w) | O(1) | O(1) | O(V)
adj list |  O(V+E) | O(d(v)) | O(d(v)) | O(d(v))

adj list (y)  
digraph ehhhh -- sparse

## Directed Acyclic Graph (DAG)
_Is it a tree? Is it a graph? No it's a dag!_  
DAGs are a type of tree/graph, where nodes don't go back

# Weighted Graphs
Some nodes have a higher (or lower) associated weight/value

## Theoretical Implementation
Adjacency Matrix - store the weight rather than just true/false  
_- may need some 'no edge exists' value, as zero might be a valid weight_

Adjacency List - add weight to each list node

Edge list - add weight to each edge

Linked data structure - links become link/weight pairs

### Shortest Path Search
- Find minimum cost path

source-target - shortest from v->w
single-source - from v to all
all-pairs - (v,w) [ minimum for all pairs ]

### Single Source Shortest Path Search
O(nlogv) - adjaceny matrix and heap
for an adjacency matrix (O(V^2))

**Given a counter; the first time a node is visited is the earliest time it could be visited**

# Minumum Spanning Trees (MST)
Reminder: A spanning tree is a acyclic graph with all vertices of a graph.  
A minimum spanning tree is the smallest representation of this subgraph.

## Kruskal&apos;s Algorithm
= take all edges, sorted according to their weight; and for each add it to the prototype MST unless it will introduce a cycle
[A, B, C, D, E, F, G, H]
{A, B, C, -, E, -, -, H}

** Cycle checking is expensive - (dfs everything!?)
--> O(ElogE)

## Prim&apos;s Algorithm
(Prim, Jarnik and Dijkstra)

Start from any vertex with an MST
Choose an edge not already in the MST, that
- does not cause a cycle
- connects to a vertex on the mst ('on the fringe')
- minimum possible edge

--> Choose the smallest edge from any of the vertices in the MST that does not cause a cycle

// and takes us somewhere new

// **But lol they're the same!** --> Prim == Jarnik == Dijkstra
(pick a new edge, minimum, no cycle)

## Demo
--> redblobgames.com/pathfinding/a-star/introduction.html

