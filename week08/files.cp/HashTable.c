// HashTable.c ... implementation of HashTable ADT
// Written by John Shepherd, May 2013
// Modified by $name, January 2019

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "HashTable.h"
#include "List.h"

// Types and functions local to HashTable ADT

typedef struct hash_table hash_table;
struct hash_table {
	List *lists;
	size_t nslots; // # elements in array
	size_t nitems; // # items stored in HashTable
};

static inline size_t hash (Key, size_t);

// Interface functions for HashTable ADT

// Create an empty HashTable.
hash_table *hash_table_new (size_t n)
{
	hash_table *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate hashtable");
	*new = (hash_table) {
		.nslots = n,
		.nitems = 0,
		.lists = calloc (n, sizeof (List))
	};

	for (size_t i = 0; i < n; i++)
		new->lists[i] = list_new ();
	return new;
}

// Free the memory associated with the HashTable.
void hash_table_drop (hash_table *ht)
{
	assert (ht != NULL);
	for (size_t i = 0; i < ht->nslots; i++)
		list_drop (ht->lists[i]);
	free (ht->lists);
	free (ht);
}

// Display HashTable stats.
void hash_table_print_stats (hash_table *ht)
{
	assert (ht != NULL);
	printf ("Hash Table Stats:\n");
	printf ("Number of slots = %d\n", 0); // TODO
	printf ("Number of items = %d\n", 0); // TODO
	printf ("Chain length distribution\n");
	printf ("%8s %8s\n", "Length", "#Chains");
	// TODO .. rest of function to show length/freq pairs
}

// Insert a new value into the HashTable.
void hash_table_insert (hash_table *ht, Item it)
{
	assert (ht != NULL);
	size_t i = hash (key (it), ht->nslots);
	if (list_search (ht->lists[i], key (it)) == NULL) {
		list_insert (ht->lists[i], it);
		ht->nitems++;
	}
}

// Delete a value from the HashTable.
void hash_table_delete (hash_table *ht, Key k)
{
	assert (ht != NULL);
	size_t h = hash (k, ht->nslots);
	list_delete (ht->lists[h], k);
}

// Get an Item from the HashTable using the specified Key.
Item *hash_table_search (hash_table *ht, Key k)
{
	assert (ht != NULL);
	size_t i = hash (k, ht->nslots);
	return list_search (ht->lists[i], k);
}

// Convert key into index (from Sedgewick)
static inline size_t hash (Key k, size_t table_size)
{
	register size_t h = 0;
	register unsigned a = 31415, b = 27183;
	for (; *k != '\0'; k++) {
		a = a * b % (table_size - 1);
		h = (a * h + (unsigned)*k) % table_size;
	}
	return h % table_size;
}
