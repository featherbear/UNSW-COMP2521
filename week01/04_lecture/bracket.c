#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void unmatched (int c);

int main (void)
{
	Stack s = stack_new ();

	int ch;
	while ((ch = getchar ()) != EOF) {
		switch (ch) {
		case '(': case '[': case '{':
			stack_push (s, ch);
			break;
		case ')':
			if (stack_pop (s) != '(') unmatched(ch);
			break;
		case ']':
			if (stack_pop (s) != '[') unmatched(ch);
			break;
		case '}':
			if (stack_pop (s) != '{') unmatched(ch);
			break;
		}

		putchar (ch);
	}

	if (s->n_items != 0)
		unmatched (stack_pop (s));

	stack_drop (s);
	return EXIT_SUCCESS;
}

void unmatched (int c)
{
	printf ("unmatched '%c'\n", c);
	exit (EXIT_FAILURE);
}
