////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY05_STR_A	"1.1 abc\n"
#define DRY05_STR_B	"1.2 def\n"
#define DRY05_STR_C	"xxxxx\n"
#define DRY05_STR_D	"new stuff\n"
#define DRY05_STR_E	"blah blah blah\n"
#define DRY05_STR_F	"qwer\n"
#define DRY05_STR1	DRY05_STR_A DRY05_STR_B DRY05_STR_C
#define DRY05_STR2	DRY05_STR_D DRY05_STR_E DRY05_STR_F

	Textbuffer tb1 = textbuffer_new (DRY05_STR1);
	assert (tb1 != NULL);

	Textbuffer tb2 = textbuffer_new (DRY05_STR2);
	assert (tb2 != NULL);

	{
		char *str = textbuffer_to_str (tb1);
		assert (strcmp (str, DRY05_STR1) == 0);
		free (str);
	}

	{
		char *str = textbuffer_to_str (tb2);
		assert (strcmp (str, DRY05_STR2) == 0);
		free (str);
	}

	textbuffer_insert (tb1, 1, tb2);

	{
		char *str = textbuffer_to_str (tb1);
		char *exp =
			DRY05_STR_A
			DRY05_STR_D DRY05_STR_E DRY05_STR_F
			DRY05_STR_B DRY05_STR_C;
		assert (strcmp (str, exp) == 0);
		free (str);
	}

	textbuffer_drop (tb1);

	return EXIT_SUCCESS;
}
