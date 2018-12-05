////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- A Stack ADT implementation, using arrays.
//
// 2018-11-29	Jashank Jeremy <jashankj@cse.unsw.edu.au>
// YYYY-mm-dd   Your Name Here <zNNNNNNN@student.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "item.h"
#include "stack.h"
#include "testable.h"

#define DEFAULT_SIZE 10

int max(int a, int b);

typedef struct stack stack;

struct stack {
	size_t n_items;
	size_t capacity;
	Item *items;
};

/** Create a new, empty Stack. */
stack *stack_new (void)
{
	stack *new = malloc (sizeof *new);
	if (new == NULL)
		err (EX_OSERR, "couldn't allocate stack");

	(*new) = (stack) { .n_items = 0, .capacity = DEFAULT_SIZE };

	new->items = calloc (DEFAULT_SIZE, sizeof(Item));
	if (new->items == NULL)
		err (EX_OSERR, "couldn't allocate stack items");

	return new;
}

/** Destroy a Stack. */
void stack_drop (stack *s)
{
	assert (s != NULL);
	free (s->items); // only frees one
	free (s);
}

/** Add an item to the top of a Stack. */
void stack_push (stack *s, Item it)
{
	assert (s != NULL);
	s->items[s->n_items] = it;
	s->n_items++;

	if (s->n_items == s->capacity) {
		s->capacity = s->capacity + DEFAULT_SIZE;
		s->items = realloc(s->items, s->capacity);
	}

	return;
}

int max(int a, int b) { return a > b ? a : b; }

/** Remove an item from the top of a Stack. */
Item stack_pop (stack *s)
{
	assert (s != NULL);
	if (!s->n_items) {
		fprintf(stderr, "stack underflow");
		abort();
	}
	Item it = s->items[s->n_items - 1];
	s->n_items--;

	if (s->n_items < s->capacity/4) {
		s->items = realloc(s->items, (unsigned int)max(s->capacity/2, DEFAULT_SIZE));
	}

	return it;
}

/** Get the number of items in a Stack. */
size_t stack_size (stack *s)
{
	assert (s != NULL);
	return s->n_items;
}

void white_box_tests (void)
{



}
