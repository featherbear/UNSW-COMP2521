// COMP2521 19T0 ... lab 01: a linked list implementation
//
// 2018-11-24	Jashank Jeremy <jashank.jeremy@unsw.edu.au>
// YYYY-mm-dd	Your Name Here <zNNNNNNN@student.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

// Remove `__unused' tags from your functions before flight.
#define __unused __attribute__((unused))

#include "lists.h"

/** Traverses and prints the list. */
void list_print (link list)
{
	link curr = list;
	while (curr != NULL) {
		printf ("[%d]->", curr->item);
		curr = curr->next;
	}
	puts ("|");
}

/** Create a new node, initialised with the item provided.
 * Return a pointer to node (link) */
link node_new (Item it __unused)
{
	return NULL;
}

/** Insert a new node into a given non-empty list.
 * The node is inserted directly after the head of the list. */
void list_insert_next (link list __unused, link node __unused)
{
	return;
}

/** Return the sum of all items in list */
int list_sum_items (link list __unused)
{
	return -1;
}

/** Frees all memory used in the list */
void list_drop (link list __unused)
{
	return;
}


/** Create a circular list with the specified number of nodes,
 * with each link storing data from 1 to the number of nodes. */
link clist_new (int n_nodes __unused)
{
	return NULL;
}

/** print the data in a circular fashion starting from any node */
void clist_print (link clist __unused)
{
	return;
}


/** Create a double-linked list which contains the same values,
 * in the same order as 'list' */
dlink dlist_new_from_list (link list __unused)
{
	return NULL;
}

/** Print a doubly-linked list. */
void dlist_print (dlink list __unused)
{
	return;
}

/** Frees all the memory used in the double-linked list */
void dlist_drop (dlink list __unused)
{
	return;
}
