// words.c ... Build a tree of words from /usr/dict/words
// Written by John Shepherd, August 2008
// Modified by Angela Finlayson, January 2009
// Further modified by $name, January 2019

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "Item.h"
#include "Tree.h"

#define WORD_FILE "/usr/share/dict/words"
#define MAX_WORD_SIZE BUFSIZ

int read_words_into_tree(FILE *, Tree t);

// Get words, store in tree, print tree stats
int main (int argc, char **argv)
{
	char *words_file = WORD_FILE;
	if (argc > 1)
		words_file = argv[1];

	enum tree_strategy strategy = NO_REBALANCE;
	if (argc > 2) {
		unsigned x = (unsigned) atoi (argv[2]);
		assert (NO_REBALANCE <= x && x <= SPLAY);
		strategy = (enum tree_strategy) x;
	}

	FILE *f;
	if (strcmp (words_file, "-") == 0)
		f = stdin;
	else if ((f = fopen (words_file, "r")) == NULL)
		err (EX_DATAERR, "couldn't open %s", words_file);
	else
		__builtin_unreachable ();

	Tree word_tree = tree_new (strategy);

	printf ("Reading words from %s\n", words_file);
	int num_words = read_words_into_tree (f, word_tree);
	printf ("Read in %d words\n", num_words);

	tree_print_stats (word_tree);

	// Note: you may wish to comment this out for large trees!
	tree_display (word_tree);

	tree_destroy (word_tree);
	fclose (f);
	return 0;
}

// Read all words from the file `f` into the tree `word_tree`.
int read_words_into_tree (FILE *f, Tree word_tree)
{
	int num_words = 0;
	char word[MAX_WORD_SIZE];

	while (fgets (word, MAX_WORD_SIZE, f) != NULL) {
		// Remove the trailing newline, if one exists.
		// (see `man 3 strcspn`)
		word[strcspn (word, "\n")] = '\0';

		// Only insert non-empty words.
		if (strcmp (word, "") == 0) continue;

		tree_insert (word_tree, strdup (word));
		num_words++;
		assert (tree_search (word_tree, word) == 1);
	}

	// Warning: We are assuming these strings do not occur
	// in the input
	assert (tree_search (word_tree, "!aaaaaa!") == 0);
	assert (tree_search (word_tree, "!xxxxxx!") == 0);
	assert (tree_search (word_tree, "!yyyyyy!") == 0);
	assert (tree_search (word_tree, "!zzzzzz!") == 0);
	return num_words;
}
