---
title: "Lecture 14 - Hashing"
date: 2019-01-29T14:35:02+11:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---
http://cr.yp.to/
http://www.cse.unsw.edu.au/~cs2521/18x1/exams/Algos/

# Why
Access closer to O(1) constant time

// for crypto
pre-image - Given hash(m), hard to pick m
second pre-image - hard to pick a pair of distinct ??
collision resistant - hard to find 

// mod 128 ... same as & 0b01111111

# What
* For a table of size `N`, output range from `0` - `N-1`  
* Purely deterministic (same results for `h(k,N)`)
* Uniform distribution \*
* Cheap to compute

\*: i.e. for hashing, if we are hashing by summing characters - lower hash numbers are more probable to have an 'a' in it
# How
```c
size_t hash(char *key, size_t N) {
  return strlen(key) % N;
}
```

# Hash Collision / Collision Resolution
- If two keys hash to the same value, then in our implementation we can allow multiple items in a single location (ie array of item arrays, array of linked lists)

- Probing - new indicies

- Remake the array, rehash!!! rip

//
Best and Worst Case

Given N slots and M items  
Best case - all lists have length M/N (uniformly distributed)  
Worst case - one list has length M (all the items), other lists are empty

A good hash function, when M <= N - cost O(1)
A good hash function, when M > N - cost O(M/N)

The M/N ratio is called the load (alpha)


//

Linear Probing | Open-address hashing - Using the next avalable slot
Access `n` - if full then try `n+1`, `n+2` ... 0 ... 1... 2...

Successful: 1/2 (1+1/(1-alpha))
Unsuccessful: 1/2 (1+ 1/(1-alpha)^2)

Deleting is painful.. have to remove and reinsert trailing values

// Double hashing
If position hash1(k,N) is full, then insert into hash1(k,N)+hash2(k,N)
If hash1(k,N)+hash2(k,N) is full, then try hash1(k,N)+hash2(k,N)+hash2(k,N)


Dynamic Hashtable Resizing .. increase the size.. but then have to rehash things


# Performance
gprof (compile programs with -pg)

$> gprof ./prog


lowercase ascii and uppsercase ascii differ by one bit :p (+ 32 0b00100000)

```bash
command | word 1 | distrib
```



# Hash Table ADT




```c
#include "ht.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stderr.h>
#include <sysexit.h>

struct ht {
  size_t nitems;
  size_t nslots;
  struct ht_entry {
      key key;
      value val;
  } *entries;
}

ht *ht_new (size_t size) {
  ht *new = malloc(sizeof *new);
  (*new) = (ht) {
    .nitems = 0,
    .nslots = size,
    .entries = calloc(size, sizeof(ht_entry))
  };
  return NULL;
}

void *ht_drop(size_t size) {
  // for elem in entries free key and value but ceebs
  free(ht->entries);
  free(ht)
}

void *ht_insert(ht *, key key, value val) {
  size_t h = hash(key, ht->nslots);
  assert(!ht->enties[h]);

  
  ht->entries[h] = (struct ht_entry) {
    .key = key_clone(key),
    .value = value_clone(value)
  };

  ht->nitems++;
  
}

ht select

ht select

ht delete


```

```c
typedef char *key;
typedef char *value;

key_clone --> strdup?
value_clone --> strdup?
```