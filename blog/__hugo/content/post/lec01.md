---
title: "Lecture 1"
date: 2018-11-27T14:00:00+11:00

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

`2521 3c -o prog prog.c`  
`2521 3c -o prog prog.c f2.c f3.c`  
`2521 3c + leak -o prog prog.c f2.c f3.c`  

## Switch Statements
We
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

### The default case
```c
switch() {
  case: ...
  case: ...
  default:
    ...
}
```
## Misc
* Function prototypes don't need a variable name, only the type

## break, return, continue

## function pointers
return_t (*var)(arg_t, ...)

int -> int: int (*fp)(int);
(int,int) -> void:void(*fp2)(int,int);


# dun guf
malloc size of pointer to struct
