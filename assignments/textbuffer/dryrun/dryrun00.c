////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... assignment 1: Textbuffer ADT submission tests

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

int main (void)
{
#define DRY00_STR_A	"1.1 abc\n"
#define DRY00_STR_B	"1.2 def\n"
#define DRY00_STR	DRY00_STR_A DRY00_STR_B

	Textbuffer tb = textbuffer_new (DRY00_STR);
	assert (tb != NULL);

	textbuffer_drop (tb);

	return EXIT_SUCCESS;
}
