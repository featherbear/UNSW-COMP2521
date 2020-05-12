////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY08_STR_A	"line 00\n"
#define DRY08_STR_B	"line 01\n"
#define DRY08_STR_C	"line 02\n"
#define DRY08_STR_D	"line 03\n"
#define DRY08_STR_E	"line 04\n"
#define DRY08_STR_F	"line 05\n"
#define DRY08_STR_G	"line 06\n"
#define DRY08_STR_H	"line 07\n"
#define DRY08_STR_I	"line 08\n"
#define DRY08_STR_J	"line 09\n"
#define DRY08_STR \
	DRY08_STR_A DRY08_STR_B DRY08_STR_C DRY08_STR_D DRY08_STR_E \
	DRY08_STR_F DRY08_STR_G DRY08_STR_H DRY08_STR_I DRY08_STR_J
#define DRY08_STR1 \
	DRY08_STR_D DRY08_STR_E DRY08_STR_F DRY08_STR_G DRY08_STR_H \
	DRY08_STR_I DRY08_STR_J
#define DRY08_STR2 \
	DRY08_STR_A DRY08_STR_B DRY08_STR_C

	Textbuffer tb1 = textbuffer_new (DRY08_STR);
	assert (tb1 != NULL);

	{
		char *str = textbuffer_to_str (tb1);
		assert (strcmp (str, DRY08_STR) == 0);
		free (str);
	}

	Textbuffer tb2 = textbuffer_cut (tb1, 0, 2);
	assert (tb2 != NULL);
	assert (textbuffer_lines (tb1) == 7);
	assert (textbuffer_bytes (tb1) == 56);
	assert (textbuffer_lines (tb2) == 3);
	assert (textbuffer_bytes (tb2) == 24);

	{
		char *str = textbuffer_to_str (tb1);
		assert (strcmp (str, DRY08_STR1) == 0);
		free (str);
	}

	{
		char *str = textbuffer_to_str (tb2);
		assert (strcmp (str, DRY08_STR2) == 0);
		free (str);
	}

	textbuffer_drop (tb1);
	textbuffer_drop (tb2);

	return EXIT_SUCCESS;
}
