////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY10_STR_A	"1 line 00\n"
#define DRY10_STR_B	"1 line 01\n"
#define DRY10_STR_C	"1 line 02\n"
#define DRY10_STR_D	"1 line 03\n"
#define DRY10_STR_E	"1 line 04\n"

#define DRY10_STR_G	"2 line 00\n"
#define DRY10_STR_H	"2 line 01\n"
#define DRY10_STR_I	"2 line 02\n"
#define DRY10_STR_J	"2 line 03\n"

#define DRY10_STR1 \
	DRY10_STR_A DRY10_STR_B DRY10_STR_C DRY10_STR_D DRY10_STR_E
#define DRY10_STR2 \
	DRY10_STR_G DRY10_STR_H DRY10_STR_I DRY10_STR_J

	////////////////////////////////////////////////////////////////
	{
		Textbuffer tb1 = textbuffer_new (DRY10_STR1);
		assert (tb1 != NULL);

		textbuffer_swap (tb1, 0, 2);
		textbuffer_swap (tb1, 1, 3);
		textbuffer_swap (tb1, 2, 4);

		{
			char *str = textbuffer_to_str (tb1);
			char *exp =
				DRY10_STR_C DRY10_STR_D DRY10_STR_E
				DRY10_STR_B DRY10_STR_A;
			assert (strcmp (str, exp) == 0);
			free (str);
		}

		textbuffer_undo (tb1);
		textbuffer_undo (tb1);
		textbuffer_undo (tb1);

		{
			char *str = textbuffer_to_str (tb1);
			assert (strcmp (str, DRY10_STR1) == 0);
			free (str);
		}

		textbuffer_redo (tb1);
		textbuffer_redo (tb1);

		{
			char *str = textbuffer_to_str (tb1);
			char *exp =
				DRY10_STR_C DRY10_STR_D DRY10_STR_A
				DRY10_STR_B DRY10_STR_E;
			assert (strcmp (str, exp) == 0);
			free (str);
		}

		textbuffer_drop (tb1);
	}

	////////////////////////////////////////////////////////////////
	{
		Textbuffer tb1 = textbuffer_new (DRY10_STR1);
		assert (tb1 != NULL);

		textbuffer_delete (tb1, 3, 4);
		textbuffer_delete (tb1, 0, 1);

		{
			char *str = textbuffer_to_str (tb1);
			assert (strcmp (str, DRY10_STR_C) == 0);
			free (str);
		}

		textbuffer_undo (tb1);

		{
			char *str = textbuffer_to_str (tb1);
			char *exp =
				DRY10_STR_A DRY10_STR_B DRY10_STR_C;
			assert (strcmp (str, exp) == 0);
			free (str);
		}

		textbuffer_undo (tb1);

		{
			char *str = textbuffer_to_str (tb1);
			assert (strcmp (str, DRY10_STR1) == 0);
			free (str);
		}

		textbuffer_drop (tb1);
	}

	////////////////////////////////////////////////////////////////
	{
		Textbuffer tb1 = textbuffer_new (DRY10_STR1);
		assert (tb1 != NULL);

		Textbuffer tb2 = textbuffer_new (DRY10_STR2);
		assert (tb2 != NULL);

		textbuffer_paste (tb1, 3, tb2);

		{
			char *str = textbuffer_to_str (tb1);
			char *exp =
				DRY10_STR_A DRY10_STR_B DRY10_STR_C
				DRY10_STR_G DRY10_STR_H DRY10_STR_I DRY10_STR_J
				DRY10_STR_D DRY10_STR_E;
			assert (strcmp (str, exp) == 0);
			free (str);
		}

		textbuffer_undo (tb1);

		{
			char *str = textbuffer_to_str (tb1);
			assert (strcmp (str, DRY10_STR1) == 0);
			free (str);
		}

		textbuffer_insert (tb1, textbuffer_lines (tb1), tb2);

		{
			char *str = textbuffer_to_str (tb1);
			char *exp = DRY10_STR1 DRY10_STR2;
			assert (strcmp (str, exp) == 0);
			free (str);
		}

		textbuffer_redo (tb1);

		{
			char *str = textbuffer_to_str (tb1);
			char *exp = DRY10_STR1 DRY10_STR2;
			assert (strcmp (str, exp) == 0);
			free (str);
		}

		textbuffer_undo (tb1);

		{
			char *str = textbuffer_to_str (tb1);
			assert (strcmp (str, DRY10_STR1) == 0);
			free (str);
		}

		textbuffer_drop (tb1);
	}

	////////////////////////////////////////////////////////////////
	{
		Textbuffer tb1 = textbuffer_new (DRY10_STR1);
		assert (tb1 != NULL);

		{
			char *str = textbuffer_to_str (tb1);
			assert (strcmp (str, DRY10_STR1) == 0);
			free (str);
		}

		Textbuffer tbx = textbuffer_cut (tb1, 3, 4);
		Textbuffer tby = textbuffer_cut (tb1, 0, 1);

		{
			char *str = textbuffer_to_str (tb1);
			assert (strcmp (str, DRY10_STR_C) == 0);
			free (str);
		}

		textbuffer_undo (tb1);
		textbuffer_undo (tb1);

		{
			char *str = textbuffer_to_str (tb1);
			assert (strcmp (str, DRY10_STR1) == 0);
			free (str);
		}

		textbuffer_redo (tb1);

		{
			char *str = textbuffer_to_str (tb1);
			char *exp = DRY10_STR_A DRY10_STR_B DRY10_STR_C;
			assert (strcmp (str, exp) == 0);
			free (str);
		}

		textbuffer_drop (tbx);
		textbuffer_drop (tby);
		textbuffer_drop (tb1);
	}

	return EXIT_SUCCESS;
}
