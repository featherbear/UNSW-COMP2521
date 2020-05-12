////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY07_STR_A	"1.1 abc\n"
#define DRY07_STR_B	"def\n"
#define DRY07_STR_C	"ghc\n"
#define DRY07_STR_D	"klmnbc\n"
#define DRY07_STR	DRY07_STR_A DRY07_STR_B DRY07_STR_C DRY07_STR_D

	Textbuffer tb = textbuffer_new (DRY07_STR);
	assert (tb != NULL);

	{
		char *str = textbuffer_to_str (tb);
		assert (strcmp (str, DRY07_STR) == 0);
		free (str);
	}

	textbuffer_replace (tb, "bc", "YY");

	{
		char *str = textbuffer_to_str (tb);
		char *exp =
			"1.1 aYY\n"
			DRY07_STR_B DRY07_STR_C
			"klmnYY\n";
		assert (strcmp (str, exp) == 0);
		free (str);
	}

	textbuffer_drop (tb);

	return EXIT_SUCCESS;
}
