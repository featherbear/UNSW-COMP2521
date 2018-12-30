---
title: "Lecture 1"
date: 2018-11-27T14:00:00+11:00
description: "3c, switch, misc, gdb"

hiddenFromHomePage: false
postMetaInFooter: false

categories: ["Lectures"]

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

# PSA: A new compiler has entered the game
Okay not really, it's the lecturer's wrapper for `clang`, which has pretty love messages (error messages in red) when things go wrong.

## Compiling a single file
`2521 3c -o prog prog.c`  

## Compiling from multiple files
`2521 3c -o prog prog.c f2.c f3.c`  

## Compiling with leak checking
`2521 3c +leak -o prog prog.c f2.c f3.c`  


# Switch Statements
```c
switch (variable) {
  case a:
    ...
    break;
  case b:
    ...
    break;
  case c:
    ...
    break;
}
```

Equivalent to 
```c
if (variable == a) {
  ...
} else if (variable == b) {
  ...
} else if (variable == c) {

}
```

## The default case
```c
switch() {
  case: ...
  case: ...
  default:
    ...
}
```


# Misc
* Function prototypes don't need a variable name, only the type

## break, continue, return
* `break` - exit a loop
* `continue` - steps through the loop to the next iteration
* `return` - exit a function (exits a loop implicitly)

## function pointers
return_t (*var)(arg_t, ...)  

int -> int: int (*fp)(int);  
(int, int) -> void: void(*fp2)(int,int);  


## When `malloc`'ing structs
If we're using a pointer type, remember to pass in the actual struct to malloc

```c
struct st;
typedef struct st *St;

/* 1 */ St st_ptr = malloc(sizeof(St));      // Will ONLY malloc 4 bytes (sizeof a pointer)
/* 2 */ St st_ptr = malloc(sizeof(*st_ptr)); // Would malloc the space we need
```

* Can't dereference a typedef'd struct
* `#2` - `sizeof(*st_ptr)` -> dereferencing during declaration to give us `struct st`

# And a summary of a summary of gdb
![](../gdb.png)