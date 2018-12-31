---
title: "Lecture 4 - Applications of Recursion"
date: 2018-12-06T10:16:32+11:00
description: "Recursive Linked Lists, Binary Trees"
categories: ["Lectures"]

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: true
  options: "{theme: 'hand'}"

---

# Recursive Linked Lists
## Drop a list
```c
void list_drop_recursive(node *curr) {
  if (curr == NULL) return;
  list_drop_recursive(curr->next);
  free(curr);
}
```

## Delete a node from a list
```c
void list_delete_recursive(node *curr, node *it) {
  if (curr == NULL) return NULL;
  if (curr == it) {
    node next = curr->next;
    free(curr);
    return next;
  }
  curr->next = list_delete_node(curr->next, it);
  return curr;
}
```

# Max value in an array (Divide and Conquer)
## Iterative approach
Complexity: O(n)  
```c
int array_max(int a[], size_t n) {
  int max = a[0];
  for (size_t i = 0; i <; i++) {
    if (a[i] > max) max = a[i];
  }
}
```

## Recursive approach
Complexity: O(n) _?_
```c
int array_max(int a[], size_t l, size_t, r) {
  if (l == r) return a[l];
  int m = (l + r) / 2;
  int m1 = array_max(a, l, m);
  int m2 = array_max(a, m+1, r);
  return m2 > m1 ? m2: m1;
}
```

<!-- map-reduce -->

