// HashTable.h ... interface to HashTable ADT
// Written by John Shepherd, March 2013
// Modified by $name, January 2019

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include "Item.h"

typedef struct hash_table *HashTable;

// Create an empty HashTable.
HashTable hash_table_new(size_t);
// Free the memory associated with the HashTable.
void hash_table_drop(HashTable);
// Display HashTable stats.
void hash_table_print_stats(HashTable);

// Insert a new value into the HashTable.
void hash_table_insert(HashTable, Item);
// Delete a value from the HashTable.
void hash_table_delete(HashTable, Key);
// Get an Item from the HashTable using the specified Key.
Item *hash_table_search(HashTable, Key);

#endif
