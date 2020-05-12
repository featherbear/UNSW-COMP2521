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
	struct ht_entry { key key; value value; } *entries;
};

ht *ht_new (size_t size)
{
	ht *new = malloc (sizeof *new);
	*new = (ht) {
		.nitems = 0,
		.nslots = size,
		.entries = calloc (size, sizeof (ht_entry))
	};
	return new;
}

void ht_insert (ht *ht, key key, value value)
{
	size_t hash = key_hash (key, ht->nslots);
	assert (ht->entries[hash].key == NULL);

	ht->entries[hash] = (ht_entry) {
		.key = key_clone (key),
		.value = value_clone (value),
	};
	ht->nitems++;
}

static void ht_resize (ht *ht, size_t old, size_t new)
{
	ht_entry *old_entries = ht->entries;
	ht->entries = calloc (new, sizeof (ht_entry));
	ht->nslots = new; ht->nitems = 0;
	for (size_t i = 0; i < old; i++) {
		ht_insert (ht, old_entries[i]);
	}
	free (old_entries);
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
		ht_entry *curr = &ht->entries[i];
		printf ("\t\t{ '%s' => '%s' }\n", curr->key, curr->value);
	}
}
