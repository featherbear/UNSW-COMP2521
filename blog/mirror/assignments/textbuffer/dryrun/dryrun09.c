////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY09_STR_A	"line 00\n"
#define DRY09_STR_B	"line 01\n"
#define DRY09_STR_C	"line 02\n"
#define DRY09_STR_D	"line 03\n"
#define DRY09_STR_E	"line 04\n"
#define DRY09_STR_F	"line 05\n"
#define DRY09_STR_G	"line 06\n"
#define DRY09_STR_H	"line 07\n"
#define DRY09_STR_I	"line 08\n"
#define DRY09_STR_J	"line 09\n"
#define DRY09_STR \
	DRY09_STR_A DRY09_STR_B DRY09_STR_C DRY09_STR_D DRY09_STR_E \
	DRY09_STR_F DRY09_STR_G DRY09_STR_H DRY09_STR_I DRY09_STR_J
#define DRY09_STR2 \
	DRY09_STR_A DRY09_STR_B DRY09_STR_C

	Textbuffer tb1 = textbuffer_new (DRY09_STR);
	assert (tb1 != NULL);

	{
		char *str = textbuffer_to_str (tb1);
		assert (strcmp (str, DRY09_STR) == 0);
		free (str);
	}

	Textbuffer tb2 = textbuffer_copy (tb1, 0, 2);
	assert (tb2 != NULL);
	assert (textbuffer_lines (tb1) == 10);
	assert (textbuffer_bytes (tb1) == 80);
	assert (textbuffer_lines (tb2) == 3);
	assert (textbuffer_bytes (tb2) == 24);

	{
		char *str = textbuffer_to_str (tb1);
		assert (strcmp (str, DRY09_STR) == 0);
		free (str);
	}

	{
		char *str = textbuffer_to_str (tb2);
		assert (strcmp (str, DRY09_STR2) == 0);
		free (str);
	}

	textbuffer_drop (tb1);
	textbuffer_drop (tb2);

	return EXIT_SUCCESS;
}
