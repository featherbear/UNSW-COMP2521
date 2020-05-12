// List.h ... interface to List GADT
// Written by John Shepherd, March 2013
// Modified by $name, January 2019

#ifndef LIST_H
#define LIST_H

#include "Item.h"
#include <stdlib.h>

typedef struct list *List;
typedef struct list_node *ListNode;

// Create a new, empty list.
List list_new(void);

// Free the memory used by the list.
void list_drop(List);

// Display the list as [1, 2, 3, ...].
void list_show(List);

// Add an item to the list.
void list_insert(List, Item);

// Remove an item from the list.
void list_delete(List, Key);

// Return the item with the given key.
Item *list_search(List, Key);

// Count the number of items in the list.
size_t list_length(List);

#endif
