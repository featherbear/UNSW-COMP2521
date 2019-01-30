---
title: "Lecture 9 - Sorting"
date: 2019-01-15T14:19:20+11:00
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


Sorting is arranging a collection of items in order, based on some key

# Uses
* Speeds up searches
* Intermediary for other algorithms)

'stable sort' - ordering is preserved for equivalent data  
'adaptive sorting' - sorting is differently affected by properties of the input (size, already sorted?)  
'in-place sort' - modifying the items in an array directly  

# Complexity
N - number of items (hi - lo + 1)  
C - number of comparisons between items  
S - number of swaps  

<s>
Random - avg case  
Sort ASC - best  
Sort DESC - worst  
</s>

# The Sorts
## Bubble Sort
Compares two adjacent items, swap if necessary.    
Repeat until all items are adjacent  

Complexity  
(N^2 + N)/2 - 1     (Arithmetic Progression -> S_n = n/2 * (a+l))  
Comparison: (N^2 - N)/2  
Swaps: (N^2 - N)/2  

O(n^2)

// Stable, in-place, non-adaptive ()
Bubble sort will always be N^2 regardless of the nature of the data

## Bubble Sort (Early Exit)
After a pass, if no items were sorted in that pass - the sort can exit.  
(No items being sorted indicate that the array is in order)

## Selection Sort
Pick the smallest, swap with 0th item.  
Pick the second smallest, swap with 1st item.  
etc 

N + N-1 + N-2 + ... + !
1/2 N(N+1)

O(n^2)

Unstable!

## Insertion Sort
Find the item and figure out where to put it (< n)

### Complexity
worst(N) = 1 + 2+ 3+ ... + N = N/2 (N+1)
best(N) = 1 + 1 + 1 + 1 = N


Stable!


## Shell Sort
Sort mod a,
then sort mod b,
etc

h --> 1

What h to start on? 

### Complexity
< O(n^2)

~ O(n^3/2)

### Code.
```c
void sort_shell(Item items[], size_t lo, size_t hi) {
  size_t n = hi - lo + 1;
  size_t h = 1
  for (; h <= (n-1) / 9; h = (3*h) + 1 ) {

  }

  for (; h > 0; h /= 3) {
    // insertion sort
    for (size_t i = h; i < n; i++) {
        Item item = items[i];
        size_t j = i;
        for (; j > h && item  < items[j-h]; j -= h) {
          items[j] = items[j-h];
        }
        items[j] = item;
    }
  }
}

```

