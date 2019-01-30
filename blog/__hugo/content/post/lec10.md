---
title: "Lecture 10 - More sorting!"
date: 2019-01-17T10:20:58+11:00
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

Sorted Merge

Pick the smaller of two lists and continually copy

Divide and Conquer

partition the input [two equal sized parts]
recursively sort each of the partitions
merge the partitions
#mergeSort

eg
```
4 1 7 3 8 6 5 2
---------------
4 1 7 3      8 6 5 2
4 1    7 3    8 6     5 2


4    1     7    3    8    6    5    2 //lists of size one! all in order... xd

^    ^     ^    ^    ^    ^    ^    ^
[1][4]     [3][7]    [6][8]    [2][5]

^^^^^^^^^^^^^^^^^    ^^^^^^^^^^^^^^^^
[1347]               [2568]

// find the smallest of the list
1234567 8


------
```

Time Complexity

Split arrays in two halves, constant
recombine -- N

T(N) = N + 2T(N/2)

let N := 2^N
T(2^N) = 2^N + 2T(2^N / 2)
       = 2^N + 2T(2^(N-1))

divide by 2^N

T(2^N) / 2^N = 1 + 2T(2^(N-1)) / 2^N
            = 1 + T(2^(N-1))/ 2^(N-1)

...

= N

T(2^N) = 2^N N
T(N) = N log_2 N
```

```


Bottom-Up Merge Sort
// a non-recursive 
// linear n log n
useful if more data > memory available




Quick Sort 
Best pivot?? Median value
Median of Three partitinoning
Pick 3 values
pick the median








quicksort > mergesort generally


3 items, 3! = 6 permutations



