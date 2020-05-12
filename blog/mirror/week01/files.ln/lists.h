// // // // // // // DO NOT MODIFY THIS FILE! // // // // // // // // //
// COMP2521 19T0 ... lab 01 provided code
//
// Singly- and doubly-linked list implementations.

#ifndef CS2521__LAB01_LISTS_H_
#define CS2521__LAB01_LISTS_H_

#include <stdlib.h>

typedef int Item;

typedef struct node *link;
typedef struct node {
	Item item;
	link next;
} node;

typedef struct dnode *dlink;
typedef struct dnode {
	Item item;
	dlink prev;
	dlink next;
} dnode;

/** Traverses and prints the list. */
void list_print (link list);

/** Create a new node, initialised with the item provided.
 * Return a pointer to node (link) */
link node_new (Item it);

/** Insert a new node into a given non-empty list.
 * The node is inserted directly after the head of the list. */
void list_insert_next (link list, link node);

/** Return the sum of all items in list */
int list_sum_items (link list);

/** Frees all memory used in the list */
void list_drop (link list);


/** Create a circular list with the specified number of nodes,
 * with each link storing data from 1 to the number of nodes. */
link clist_new (int n_nodes);

/** print the data in a circular fashion starting from any node */
void clist_print (link clist);


/** Create a double-linked list which contains the same values,
 * in the same order as 'list' */
dlink dlist_new_from_list (link list);

/** Print a doubly-linked list. */
void dlist_print (dlink list);

/** Frees all the memory used in the double-linked list */
void dlist_drop (dlink list);


#endif // !defined(CS2521__LAB01_LISTS_H_)
