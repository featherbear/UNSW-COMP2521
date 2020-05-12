#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <sysexits.h>

#include "stack.h"

typedef struct stack stack;
typedef struct stack_item stack_item;

struct stack {
	struct stack_item {
		Item item;
		struct stack_item *next;
	} *top;
	size_t n_items;
};

/** Create a new, empty Stack. */
stack *stack_new (void)
{
	stack *new = calloc (1, sizeof (stack));
	if (new == NULL) err (EX_OSERR, "couldn't allocate stack");
	return new;
}

/** Destroy a Stack, releasing all resources associated with it. */
void stack_drop (stack *s)
{
	assert (s != NULL);

	stack_item *tmp, *curr = s->top;
	while (curr != NULL) {
		tmp = curr->next;
		free (curr);
		curr = tmp;
	}

	free (s);
}

/** Remove an item from the top of a Stack. */
Item stack_pop (stack *s)
{
	assert (s != NULL);
	assert (s->top != NULL);
	stack_item *top = s->top;
	Item it = top->item;
	s->top = top->next;
	free (top);
	s->n_items--;
	return it;
}

/** Add an item to the top of a Stack. */
void stack_push (stack *s, Item it)
{
	assert (s != NULL);
	stack_item *new = malloc (sizeof (stack_item));
	if (new == NULL) err (EX_OSERR, "couldn't allocate stack_item");
	(*new) = (stack_item){ .item = it, .next = s->top };
	s->top = new;
	s->n_items++;
}

/** Get the number of items in a Stack. */
size_t stack_size (stack *s)
{
	return s->n_items;
}
