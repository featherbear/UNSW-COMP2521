---
title: "Tutorial One"
date: 2018-11-27T17:05:45+11:00

hiddenFromHomePage: false
postMetaInFooter: false

categories: ["Tutorial / Labs"]

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---


> this is my name  
> this is my email  
> this is my face  
> this is my intro  

Stealing this thanks Gal ~

---

This tut/lab focused on revising linked lists, circular linked lists and double linked lists.

---

# Duplicating / cloning a linked list
```c
// Creates a new node, initialised with the item provided.
// Returns pointer to node (link).
link node_new (Item item)
{
    link n = malloc (sizeof (*n));
    if (n == NULL)
        err (1, "couldn't allocate");
    n->item = item;
    n->next = NULL;
    return n;
}

// Insert a new node into a given non-empty list.
// The node is inserted directly after the head of the list ls.
void list_insert_next (link ls, link node)
{
    assert (ls != NULL);
    assert (node != NULL);
    node->next = ls->next;
    ls->next = node;
}


/* ... */

link duplicateList(link ls) {
  // if given an empty list, return NULL
  if (!ls) return NULL;

  // Create a new node with the item of the frst item
  link head = node_new(ls->item);
  
  // Set pointer to the last element of the new list
  link ptr = head;

  // Set a pointer to the next element of the original list
  link cursor = head->next;

  // Move through the list and clone the items
  while (cursor) {
    list_insert_next(node_new(cursor->item), ptr)  
    ptr = ptr -> next;
    cursor = cursor -> next;
  }

  return head;
}
```

# Reversing a linked list
```c
int main (int argc, char *argv[])
{
    link ls = list_from_cstr ("hello world!");
    link ls2 = list_duplicate (ls);

    list_print (ls);
    list_print (ls2);
    list_print (list_reverse (ls));
    list_print (ls);
    list_print (ls2);

    return 0;
}
```

hello world!  
hello world!  
!dlrow elloh  
h             <-- ls1 still points to the h node
hello world!  


# Double linked list
_e.g linked lists with `next` and `prev`_

```c
typedef char Item;
typedef struct dnode *dlink;

struct dnode {
    Item item;
    dlink prev, next;
};

```

> Write a function append which attaches the list list2 at the end of list1 and returns the resulting list.
```c
dlink append (dlink list1, dlink list2) {
  if (!list1) return list2;
  if (!list2) return list1;

  dlink last = list1;
  for(; last->next; last=last->next); // while (last->next) last = last->next;
  
  last->next = list2;
  list2->prev = last;

  return list1;
}
```

> Is it necessary to return the resulting list? Could we instead get away with the following interface?
`void append (dlink list1, dlink list2);`

-- We could.

---

# Switch statements
switch(variable){...}
--> if A else if B else if C else if D else ...

## The `break` statement; execution fall-through
`break` exits a switch statement.  

If it is not present, the execution can continue into other statements.  
This could be beneficial in some cases

```c
speed = 200;
switch (speed) {
  case 200:
    printf("You're going REALLY REALLY fast\n")
  case 150:
    printf("You're going REALLY fast\n")
  case 100:
    printf("On your green Ps?\n")
  case 90:
    printf("Hah learner's license\n")
  case 40:
    printf("ScHoOl ZoNe SpEeD\n")
}
```

You're going REALLY REALLY fast  
You're going REALLY fast  
On your green Ps?  
Hah learner's license  
ScHoOl ZoNe SpEeD

## `return` (functions only)
We can use a `return` statement in place of a break if our switch statement is the only code in a function

## switch vs if/else

### Using switch
```c
assert (islower (ch));
switch (ch) {
case 'a':
case 'e':
case 'i':
case 'o':
case 'u':
    printf ("vowel");
    break;
default:
    printf ("consonant");
    break;
}
```

### Using if else
```c
assert (islower (ch));
if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') printf("vowel");
else printf ("consonant");
```


# Ternary operator
`condition ? valueIfTrue : valueIfFalse`
```c
if (condition) {
  return valueIfTrue;
} else {
  return valieIfFalse;
}
```

# Sorting
## Efficiency 
### is sorted
-> find the first occurence where something is NOT sorted

