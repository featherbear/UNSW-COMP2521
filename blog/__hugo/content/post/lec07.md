---
title: "Lecture 7 - "
date: 2019-01-03T10:00:00+11:00
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

Edge Lists

# Hamilton Paths and Tours
A **Hamilton Path** is a simple path connecting two vertices that visits each vertex in the graph exactly once.  

A **Hamilton Tour** is a cycle that visits each vertex in the graph exactly once (and comes back to itself)
^ [OneTouch Drawing - Ecapyc](https://play.google.com/store/apps/details?id=com.ecapycsw.onetouchdrawing)

> How do we check if such a path or tour exists

> How do we check how many paths exist



idea brute force!
enumerate every possible path, and check each one.
hack a bfs or dfs to do it:
keep a counter of vertices visited in the current path;
only accept a path only if count is
equal to the number of vertices.
problem how many paths?
given a simple path:
no path from t to w implies no path from v to w via t…
so there’s no point visiting a vertex twice on a simple search
… but that’s not true for a Hamilton path

we must inspect every possible path in the graph.
in a complete graph, we have V! different paths (≈ (V/e)^V
)
there are well-known, well-defined subsets of this problem
which are easy to solve (Dirac, Ore) … but in general
this is a non-deterministic polynomial, or np problem


An **Euler Path** visits every edge once if the graph is connected, and it has exactly two vertices of odd degree

An **Euler Tour** visits every edge once if the graph is connected, and all of its vertices are of even degree!

• tractable: can we find a simple path connecting two vertices in a graph?
tractable: what’s the shortest such path?
intractable: what’s the longest such path?

• tractable: is there a clique in a given graph?
intractable: what’s the largest clique?

• tractable: given two colours, can we colour every vertex in a graph
such that no two adjacent vertices are the same colour?
intractable: what about three colours?