// COMP2521 19T0 ... lab 01: a linked list implementation
//
// 2018-11-24	Jashank Jeremy <jashank.jeremy@unsw.edu.au>
// YYYY-mm-dd	Your Name Here <zNNNNNNN@student.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

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
link node_new (Item it)
{
	struct node *new = malloc (sizeof *new);
	if (new == NULL)
		err (EX_OSERR, "couldn't allocate %zu bytes", sizeof *new);
	(*new) = (struct node){ .item = it, .next = NULL };
	return new;
}

/** Insert a new node into a given non-empty list.
 * The node is inserted directly after the head of the list. */
void list_insert_next (link list, link node)
{
	assert (list != NULL);
	assert (node != NULL);

	node->next = list->next;
	list->next = node;
}

/** Return the sum of all items in list */
int list_sum_items (link list)
{
	int sum = 0;
	for (link curr = list; curr != NULL; curr = curr->next)
		sum += curr->item;
	return sum;
}

/** Frees all memory used in the list */
void list_drop (link list)
{
	for (link tmp, curr = list;
		 curr != NULL;
		 curr = tmp)
	{
		tmp = curr->next;
		free (curr);
		if (tmp == list) break;
	}
}


/** Create a circular list with the specified number of nodes,
 * with each link storing data from 1 to the number of nodes. */
link clist_new (int n_nodes)
{
	assert (n_nodes >= 0);

	link head = NULL;
	link prev = NULL;

	for (int i = 1; i <= n_nodes; i++) {
		link node = node_new ((Item) i);
		if (head == NULL)
			head = prev = node;

		prev->next = node;
		node->next = head;
		prev = node;
	}

	return head;
}

/** print the data in a circular fashion starting from any node */
void clist_print (link clist)
{
	for (link curr = clist; curr != NULL; curr = curr->next) {
		printf ("[%d]->", curr->item);
		if (curr->next == clist) break;
	}
	puts ("...");
}


static dlink dnode_new (Item it)
{
	struct dnode *new = malloc (sizeof *new);
	if (new == NULL)
		err (EX_OSERR, "couldn't allocate %zu bytes", sizeof *new);
	(*new) = (struct dnode){ .item = it, .next = NULL, .prev = NULL };
	return new;
}

/** Create a double-linked list which contains the same values,
 * in the same order as 'list' */
dlink dlist_new_from_list (link list)
{
	dlink dhead = NULL;
	dlink dtail = NULL;

	for (link curr = list; curr != NULL; curr = curr->next) {
		dlink dnode = dnode_new (curr->item);
		if (dhead == NULL) {
			dhead = dnode;
		} else {
			dtail->next = dnode;
			dnode->prev = dtail;
		}
		dtail = dnode;
	}

	return dhead;
}

/** Print a doubly-linked list. */
void dlist_print (dlink list)
{
	printf ("|");
	for (dlink curr = list; curr != NULL; curr = curr->next)
		printf ("-[%d]-", curr->item);
	puts ("|");
}

/** Frees all the memory used in the double-linked list */
void dlist_drop (dlink list)
{
	for (dlink tmp, curr = list; curr != NULL; curr = tmp) {
		tmp = curr->next;
		free (curr);
	}
}
