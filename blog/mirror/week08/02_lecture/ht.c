#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "ht.h"

typedef struct ht_entry ht_entry;
struct ht {
	size_t count;
	size_t size;
	struct ht_entry {
		char *key;
		char *value;
		ht_entry *next;
	} **table;
};

ht *ht_new (size_t size)
{
	ht *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate ht");

	(*new) = (ht){
		.count = 0,
		.size = size,
		.table = calloc (size, sizeof (ht_entry *)),
	};

	if (new->table == NULL)
		err (EX_OSERR, "couldn't allocate hashtable entries");

	return new;
}

value ht_select (ht *ht, key key)
{
	assert (ht != NULL);
	assert (key != NULL);

	size_t hash = key_hash (key, ht->size);
	ht_entry **curr = &ht->table[hash];
	while (*curr != NULL) {
		if (key_eq (key, (*curr)->key))
			return (*curr)->value;
		curr = &((*curr)->next);
	}

	return NULL;
}

void ht_insert (ht *ht, key key, value value)
{
	assert (ht != NULL);
	assert (key != NULL);
	assert (value != NULL);

	// create a new entry
	ht_entry *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate ht_entry");
	(*new) = (ht_entry) {
		.key = key_clone (key),
		.value = value_clone (value),
		.next = NULL
	};

	// tail-insert into the chain
	size_t hash = key_hash (key, ht->size);
	ht_entry **curr = &ht->table[hash];
	while (*curr != NULL)
		curr = &((*curr)->next);
	(*curr) = new;

	ht->count++;
}

void ht_update (ht *ht, key key, value value)
{
	assert (ht != NULL);
	assert (key != NULL);
	assert (value != NULL);

	size_t hash = key_hash (key, ht->size);
	ht_entry **curr = &ht->table[hash];
	while (*curr != NULL) {
		if (key_eq (key, (*curr)->key)) break;
		curr = &((*curr)->next);
	}
	if (*curr == NULL) return;

	value_drop ((*curr)->value);
	(*curr)->value = value_clone (value);
}

void ht_upsert (ht *ht, key key, value value)
{
	assert (ht != NULL);
	assert (key != NULL);
	assert (value != NULL);

	// if exists, update, else insert
	if (ht_select (ht, key) != NULL)
		ht_update (ht, key, value);
	else
		ht_insert (ht, key, value);
}

void ht_delete (ht *ht, key key)
{
	assert (ht != NULL);
	assert (key != NULL);

	size_t hash = key_hash (key, ht->size);
	ht_entry *prev = NULL, *curr = ht->table[hash];

	while (curr != NULL) {
		if (key_eq (key, curr->key)) break;
		prev = curr;
		curr = curr->next;
	}

	if (prev != NULL)
		prev->next = (curr != NULL) ? curr->next : NULL;
	else
		ht->table[hash] = curr->next;

	if (curr != NULL) {
		key_drop (curr->key);
		value_drop (curr->value);
		free (curr);
		ht->count--;
	}

	return;
}

void ht_drop (ht *ht)
{
	if (ht == NULL) return;

	for (size_t i = 0; i < ht->size; i++) {
		for (ht_entry *tmp, *curr = ht->table[i];
			 curr != NULL; curr = tmp) {
			tmp = curr->next;
			key_drop (curr->key);
			value_drop (curr->value);
			free (curr);
		}
	}

	free (ht->table);
	free (ht);
}

void ht_keys (ht *ht, key *dest)
{
	assert (ht != NULL);
	assert (dest != NULL);

	size_t idx = 0;
	for (size_t i = 0; i < ht->size; i++)
		for (ht_entry *curr = ht->table[i];
			 curr != NULL; curr = curr->next)
			dest[idx++] = curr->key;

	assert (idx <= ht->count);
}

size_t ht_count (ht *ht)
{
	assert (ht != NULL);
	return ht->count;
}


void ht_show (ht *ht)
{
	assert (ht != NULL);
	printf ("ht<%p>, contains %zu items\n", (void *)ht, ht->count);
	for (size_t i = 0; i < ht->size; i++) {
		printf ("\t[%zu] =\n", i);
		for (ht_entry *curr = ht->table[i];
			 curr != NULL; curr = curr->next)
			printf ("\t\t{ '%s' => '%s' }\n", curr->key, curr->value);
	}
}
