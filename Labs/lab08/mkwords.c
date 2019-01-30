// mkwords.c ... Generate random "words"
// Written by John Shepherd, October 2015
// Modified by $name, January 2019

#include <err.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

int main (int argc, char *argv[])
{
	if (argc != 2 && argc != 3)
		errx (EX_USAGE, "usage: mkwords <N> <seed>");

	int nwords = atoi (argv[1]);
	int seed = (argc >= 3) ? atoi (argv[2]) : 42;
	srand ((unsigned) seed);

	// Array of letters, to simulate distribution of letters in English
	char *letters = "aaaabccdeeeeeefghhiiijkllmmnnoooopqrrssttuuvwxyz";
	size_t nletters = strlen (letters);

	// Generate some words
	// Each "word" consists of 2..15 random lowercase letters
	for (int i = 0; i < nwords; i++) {
		char word[17] = {0};
		size_t len = rand () % 14 + 2;
		size_t j = 0;
		for (; j < len; j++)
			word[j] = letters[(unsigned)rand() % nletters];
		word[j] = '\n';
		word[j + 1] = '\0';
		fputs (word, stdout);
	}

	return EXIT_SUCCESS;
}
