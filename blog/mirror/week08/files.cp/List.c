// list *.c ... implementation of listist ADT as linked-list
// Written by John Shepherd, March 2013
// Modified by $name, January 2019

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "List.h"

typedef struct list list;
typedef struct list_node list_node;

struct list {
	struct list_node {
		Item value;
		list_node *next;
	} *first, *last;
};

// Create a new, empty list.
list *list_new (void)
{
    list *new = malloc(sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate list");
	*new = (list) { .first = NULL, .last = NULL };
    return new;
}

// Free the memory used by the list.
void list_drop (list *list)
{
	assert (list != NULL);
	list_node *curr = list->first;
	while (curr != NULL) {
		list_node *next = curr->next;
		item_drop (curr->value);
		free (curr);
		curr = next;
	}
	free (list);
}

// Display the list as [1, 2, 3, ...].
void list_show (list *list)
{
	assert (list != NULL);
	list_node *curr = list->first;
	printf ("[");
	while (curr != NULL) {
		item_show (curr->value);
		if (curr->next != NULL)
			printf (", ");
	}
	printf ("]");
}

// Add an item to the list.
// Note: doesn't check for duplicates.
void list_insert (list *list, Item it)
{
	assert (list != NULL);

	list_node *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate list node");
	(*new) = (list_node) { .value = it, .next = NULL };

	if (list->last == NULL) {
		list->first = list->last = new;
	} else {
		list->last->next = new;
		list->last = new;
	}
}

// Remove an item from the list.
// Note: assumes no duplicates.
void list_delete (list *list, Key k)
{
	assert (list != NULL);

	list_node *prev = NULL;
	list_node *curr = list->first;
	while (curr != NULL) {
		if (eq (k, key (curr->value))) break;
		prev = curr;
		curr = curr->next;
	}

	if (curr == NULL) return;

	if (prev == NULL)
		list->first = curr->next;
	else
		prev->next = curr->next;

	free (curr);

	if (list->first == NULL)
		list->last = NULL;
}

// Return the item containing the given key.
// Returns NULL if the key isn't found.
Item *list_search (list *list, Key k)
{
	assert (list != NULL);
	list_node *curr = list->first;
	while (curr != NULL) {
		if (eq (k, key (curr->value))) {
			return &(curr->value);
		} else {
			curr = curr->next;
		}
	}
	return NULL;
}

// Count the number of items in the list.
size_t list_length (list *list)
{
	size_t n = 0;
	for (list_node *curr = list->first; curr != NULL; curr = curr->next)
		n++;
	return n;
}
