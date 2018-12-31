---
title: "Searching Algorithms"
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

# Linear Search
> Searches from 0 to n-1 (_linearly_)  
> Time Cost: O(n)

```c
bool linear_search(int array[], int length, int x) {
    for (int i = 0; i < length; i++) {
        if (array[i] == x) {
            return true;
        }
    }
    return false;
}
```

# Linear Search (ordered data)
> Searches from 0 to n-1 (_linearly_)  
> `if (x > array[i]) break;`

```c
int linear_search_ordered(int array[], int length, int x) {
    for (int i = 0; i < length; i++) {
        if (array[i] == x) {
            return true;
        }
        else if (array[i] > x) {
            return false;
        }
    }
    return false;
}

```

# Binary Search (ordered data)

* divide and conquer  
* start at the middle; and we can check which side to continue checking

> best case: t(n) ~ O(1)  
> worst case: t(N) = 1 + t(N/2) = log2(N) + 1 ~= O(log N)  
