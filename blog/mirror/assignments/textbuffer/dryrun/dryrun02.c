////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY02_STR_A	"1.1 abc\n"
#define DRY02_STR_B	"1.2 def\n"
#define DRY02_STR	DRY02_STR_A DRY02_STR_B
#define DRY02_STR2	DRY02_STR_B DRY02_STR_A

	Textbuffer tb = textbuffer_new (DRY02_STR);
	assert (tb != NULL);

	{
		char *str = textbuffer_to_str (tb);
		assert (strcmp (str, DRY02_STR) == 0);
		free (str);
	}

	textbuffer_swap (tb, 0, 1);

	{
		char *str = textbuffer_to_str (tb);
		assert (strcmp (str, DRY02_STR2) == 0);
		free (str);
	}

	textbuffer_swap (tb, 1, 0);

	{
		char *str = textbuffer_to_str (tb);
		assert (strcmp (str, DRY02_STR) == 0);
		free (str);
	}

	textbuffer_drop (tb);

	return EXIT_SUCCESS;
}
