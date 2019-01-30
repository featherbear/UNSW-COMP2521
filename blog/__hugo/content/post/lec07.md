---
title: "Lecture 7 - Paths and Tours"
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

A **Hamilton Path** visits every **vertice** once

A **Hamilton Tour** visits every **vertice** once (and comes back to itself)  

An **Euler Path** visits every **edge** once and it has exactly two vertices of odd degree

An **Euler Tour** visits every **edge** once and all of its vertices are of even degree!

---

// reminder: tractable - polynomial time / intracatable - exponential time  

• tractable: can we find a simple path connecting two vertices in a graph?  
tractable: what’s the shortest such path?  
intractable: what’s the longest such path?  

• tractable: is there a clique in a given graph?  
intractable: what’s the largest clique?  

• tractable: given two colours, can we colour every vertex in a graph  
such that no two adjacent vertices are the same colour?  
intractable: what about three colours?  