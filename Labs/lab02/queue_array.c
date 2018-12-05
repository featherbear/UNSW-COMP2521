////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- A Queue ADT implementation, using arrays.
//
// 2018-12-01	Jashank Jeremy <jashankj@cse.unsw.edu.au>
// YYYY-mm-dd   Your Name Here <zNNNNNNN@student.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "item.h"
#include "queue.h"
#include "testable.h"

#define DEFAULT_SIZE 10

typedef struct queue queue;

struct queue {
	size_t n_items;
	size_t capacity;
	Item *items;
	size_t start;
};

/** Create a new, empty Stack.  $ O(1) $. */
queue *queue_new (void)
{
	queue *new = malloc (sizeof *new);
	if (new == NULL)
		err (EX_OSERR, "couldn't allocate queue");
	(*new) = (queue) { .n_items = 0, .capacity = DEFAULT_SIZE, .start = 0 };

	new->items = calloc (DEFAULT_SIZE, sizeof(Item));
	if (new->items == NULL)
		err (EX_OSERR, "couldn't allocate queue items");

	return new;
}

/** Destroy a Queue.  $ O(1) $. */
void queue_drop (queue *q)
{
	assert (q != NULL);
	free (q->items);
	free (q);
}


/** Add an item to the end of a Queue.  $ O(1) $.
 * Sometimes referred to as "enqueue" or "unshift". */
void queue_en (queue *q, Item it)
{
	assert (q != NULL);
	assert (q->n_items < q->capacity);
	q->items[q->start + q->n_items++] = it;
}

/** Remove an item from the front of a Queue.  $ O(n) $.
 * Sometimes referred to as "dequeue" or "shift". */
Item queue_de (queue *q)
{
	assert (q != NULL);
	if (q->n_items == 0) {
		fprintf (stderr, "queue underflow!\n");
		abort();
	}

	// shift the elements across
	Item it = q->items[q->start];
	q->n_items--;

	/*
	for (size_t i = 0; i < q->n_items; i++)
		q->items[q->start + i] = q->items[q->start + i + 1];
*/

	q->start++;
	return it;
}

/** Get the number of items in a Queue.  $ O(1) $. */
size_t queue_size (queue *q)
{
	assert (q != NULL);
	return q->n_items;
}

void white_box_tests (void)
{
	// Create queue
	queue *q = queue_new();
	assert(q);
	assert(q->n_items == 0);
	assert(q->start == 0);

	// Enqueue
	queue_en(q, 5);
	assert(q->n_items == 1);
	assert(q->start == 0);
	assert(q->items[0] == 5);

	queue_en(q, 2);
	assert(q->n_items == 2);
	assert(q->start == 0);
	assert(q->items[0] == 5);
	assert(q->items[1] == 2);

	queue_en(q, 0);
	assert(q->n_items == 3);
	assert(q->start == 0);
	assert(q->items[0] == 5);
	assert(q->items[1] == 2);
	assert(q->items[2] == 0);

	assert(queue_de(q) == 5);
	assert(q->n_items == 2);
	assert(q->start == 1);
	assert(q->items[1] == 2);
	assert(q->items[2] == 0);

	assert(queue_de(q) == 2);
	assert(q->n_items == 1);
	assert(q->start == 2);
	assert(q->items[2] == 0);

	queue_en(q, 6);
	assert(q->n_items == 2);
	assert(q->start == 2);
	assert(q->items[2] == 0);
	assert(q->items[3] == 6);

	queue_drop(q);
}
