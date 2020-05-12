#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "stack.h"

static void underflow (void) __attribute__((noreturn));

int main (int argc, char *argv[])
{
	if (argc != 2)
		errx (EX_USAGE, "usage: %s 'expression'", argv[0]);

	Stack s = stack_new ();

	char *expr_ = strdup (argv[1]);
	char *expr = expr_;
	char *token;
	while ((token = strsep (&expr, " ")) != NULL) {
		int a, b;
		// ...
	}

	if (stack_size (s) == 1) {
		printf ("%d\n", stack_pop (s));
	} else {
		printf ("[non-empty stack]\n");
	}

	stack_drop (s);
	free (expr_);
	return EXIT_SUCCESS;
}

static void underflow (void)
{
	errx (EX_SOFTWARE, "underflow!");
}
