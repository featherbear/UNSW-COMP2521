////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY03_STR_A	"1.1 abc\n"
#define DRY03_STR_B	"1.2 def\n"
#define DRY03_STR	DRY03_STR_A DRY03_STR_B

	Textbuffer tb = textbuffer_new (DRY03_STR);
	assert (tb != NULL);

	{
		char *str = textbuffer_to_str (tb);
		assert (strcmp (str, DRY03_STR) == 0);
		free (str);
	}

	textbuffer_delete (tb, 1, 0);

	{
		char *str = textbuffer_to_str (tb);
		assert (strcmp (str, DRY03_STR) == 0);
		free (str);
	}

	textbuffer_delete (tb, 0, 1);

	{
		char *str = textbuffer_to_str (tb);
		assert (str == NULL || strcmp (str, "") == 0);
		free (str);
	}

	textbuffer_drop (tb);

	return EXIT_SUCCESS;
}
