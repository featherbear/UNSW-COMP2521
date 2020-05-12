////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY04_STR_A	"1.1 abc\n"
#define DRY04_STR_B	"1.2 def\n"
#define DRY04_STR_C	"xxxxx\n"
#define DRY04_STR	DRY04_STR_A DRY04_STR_B DRY04_STR_C

	Textbuffer tb = textbuffer_new (DRY04_STR);
	assert (tb != NULL);

	{
		char *str = textbuffer_to_str (tb);
		assert (strcmp (str, DRY04_STR) == 0);
		free (str);
	}

	textbuffer_delete (tb, 0, 1);

	{
		char *str = textbuffer_to_str (tb);
		assert (strcmp (str, DRY04_STR_C) == 0);
		free (str);
	}

	textbuffer_drop (tb);

	return EXIT_SUCCESS;
}
