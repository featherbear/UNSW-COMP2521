// words2.c ... Build a hashtable of words
// Written by John Shepherd, August 2008
// Modified by $name, January 2019

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "HashTable.h"
#include "Item.h"

#define WORD_FILE "/usr/share/dict/words"
#define MAX_WORD_SIZE BUFSIZ

int read_words_into_hash_table(FILE *f, HashTable hash_table);

// Get words, store in hash_table, print hashtable stats
int main (int argc, char **argv)
{
	char *words_file = WORD_FILE;
	if (argc > 1)
		words_file = argv[1];

	size_t size = 100;
	if (argc > 2)
		size = (size_t) atoi (argv[2]);

	FILE *f;
	if (strcmp (words_file, "-") == 0)
		f = stdin;
	else if ((f = fopen (words_file, "r")) == NULL)
		err (EX_DATAERR, "couldn't open %s", words_file);
	else
		__builtin_unreachable ();

	HashTable ht = hash_table_new (size);

	printf ("Reading words from %s\n", words_file);
	int num_words = read_words_into_hash_table (f, ht);
	printf ("Read in %d words\n", num_words);

	hash_table_print_stats (ht);

	hash_table_drop (ht);
	fclose (f);
	return 0;
}


// Read all words from the file `f` into the hash table `hash_table`.
int read_words_into_hash_table (FILE *f, HashTable ht)
{
	int num_words = 0;
	char word[MAX_WORD_SIZE];

	while (fgets (word, MAX_WORD_SIZE, f) != NULL) {
		// Remove the trailing newline, if one exists.
		// (see `man 3 strcspn`)
		word[strcspn (word, "\n")] = '\0';

		// Only insert non-empty words.
		if (strcmp (word, "") == 0) continue;

		hash_table_insert (ht, strdup (word));
		num_words++;
		assert (hash_table_search (ht, word) != NULL);
	}
	// Warning: We are assuming these strings do not occur
	// in the input
	assert (hash_table_search (ht, "!aaaaaa!") == 0);
	assert (hash_table_search (ht, "!xxxxxx!") == 0);
	assert (hash_table_search (ht, "!yyyyyy!") == 0);
	assert (hash_table_search (ht, "!zzzzzz!") == 0);

	return num_words;
}
