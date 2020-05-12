#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "ht.h"

typedef struct ht_entry ht_entry;
struct ht {
	size_t nitems, nslots;
	struct ht_entry {
		key key;
		value value;
		ht_entry *next;
	} **entries;
};

ht *ht_new (size_t size)
{
	ht *new = malloc (sizeof *new);
	*new = (ht) {
		.nitems = 0,
		.nslots = size,
		.entries = calloc (size, sizeof (ht_entry *))
	};
	return new;
}

void ht_insert (ht *ht, key key, value value)
{
	size_t hash = key_hash (key, ht->nslots);
	ht_entry *new = malloc (sizeof *new);
	(*new) = (ht_entry) {
		.key = key_clone (key),
		.value = value_clone (value),
		.next = ht->entries[hash],
	};
	ht->entries[hash] = new;
	ht->nitems++;
}

value ht_select (ht *ht, key key)
{
	size_t hash = key_hash (key, ht->nslots);

	for (ht_entry *curr = ht->entries[hash];
		 curr != NULL; curr = curr->next)
		if (key_eq (key, curr->key))
			return curr->value;

	return NULL;
}

void ht_delete (ht *ht, key key)
{
   	size_t hash = key_hash (key, ht->nslots);
	for (ht_entry *prev, *curr = ht->entries[hash];
		 curr != NULL; curr = curr->next) {
		prev = curr;
		if (! key_eq (key, curr->key)) continue;
		prev->next = curr->next;
		value_drop (curr->value);
		key_drop (curr->key);
		free (curr);
		curr = prev->next;
	}
}

void ht_drop (ht *ht)
{
	free (ht->entries);
	free (ht);
}

void ht_show (ht *ht)
{
	assert (ht != NULL);
	printf ("ht<%p>, contains %zu items\n", (void *)ht, ht->nitems);
	for (size_t i = 0; i < ht->nslots; i++) {
		printf ("\t[%zu] =\n", i);
		for (ht_entry *curr = ht->entries[i];
			 curr != NULL; curr = curr->next)
			printf ("\t\t{ '%s' => '%s' }\n", curr->key, curr->value);
	}
}
