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
		if (strcmp (token, "+") == 0) {
			if (stack_size (s) < 2) underflow ();
			b = stack_pop (s); a = stack_pop (s);
			stack_push (s, a + b);
		} else if (strcmp (token, "-") == 0) {
			if (stack_size (s) < 2) underflow ();
			b = stack_pop (s); a = stack_pop (s);
			stack_push (s, a - b);
		} else if (strcmp (token, "*") == 0) {
			if (stack_size (s) < 2) underflow ();
			b = stack_pop (s); a = stack_pop (s);
			stack_push (s, a * b);
		} else if (strcmp (token, "/") == 0) {
			if (stack_size (s) < 2) underflow ();
			b = stack_pop (s); a = stack_pop (s);
			stack_push (s, a / b);
		} else {
			stack_push (s, (Item) strtol (token, NULL, 10));
		}
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
