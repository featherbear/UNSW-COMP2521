---
title: "Searching Algorithms"
date: 2018-12-04T14:40:39+11:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

# Linear Search
* Searches from 0 to n-1 (_linearly_)

# Linear Search (ordered data)
* `if (k > list[n]) break;`

# Binary Search (ordered data)
* divide and conquer
* start at the middle; and we can check which side to continue checking
> best case: t(n) ~ O(1)
> worst case: t(N) = 1 + t(N/2) = log2(N) + 1 ~= O(log N)
