////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY06_STR_A	"1.1 abc\n"
#define DRY06_STR_B	"1.2 def\n"
#define DRY06_STR_C	"xxxxx\n"
#define DRY06_STR_D	"new stuff\n"
#define DRY06_STR_E	"blah blah blah\n"
#define DRY06_STR_F	"qwer\n"
#define DRY06_STR1	DRY06_STR_A DRY06_STR_B DRY06_STR_C
#define DRY06_STR2	DRY06_STR_D DRY06_STR_E DRY06_STR_F

	Textbuffer tb1 = textbuffer_new (DRY06_STR1);
	assert (tb1 != NULL);

	Textbuffer tb2 = textbuffer_new (DRY06_STR2);
	assert (tb2 != NULL);

	{
		char *str = textbuffer_to_str (tb1);
		assert (strcmp (str, DRY06_STR1) == 0);
		free (str);
	}

	{
		char *str = textbuffer_to_str (tb2);
		assert (strcmp (str, DRY06_STR2) == 0);
		free (str);
	}

	textbuffer_paste (tb1, 1, tb2);

	{
		char *str1 = textbuffer_to_str (tb1);
		char *str2 = textbuffer_to_str (tb2);
		char *exp =
			DRY06_STR_A
			DRY06_STR_D DRY06_STR_E DRY06_STR_F
			DRY06_STR_B DRY06_STR_C;
		assert (strcmp (str1, exp) == 0);
		assert (strcmp (str2, DRY06_STR2) == 0);
		free (str1);
		free (str2);
	}

	textbuffer_drop (tb1);
	textbuffer_drop (tb2);

	return EXIT_SUCCESS;
}
