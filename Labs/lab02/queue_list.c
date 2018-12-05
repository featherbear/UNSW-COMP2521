////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- A Queue ADT implementation, using linked lists.
//
// 2018-12-01	Jashank Jeremy <jashankj@cse.unsw.edu.au>
// YYYY-mm-dd	Your Name Here <zNNNNNNN@student.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "item.h"
#include "queue.h"
#include "testable.h"

typedef struct queue queue;
typedef struct queue_node queue_node;

struct queue {
    size_t n_items;
	queue_node *head, *tail;
};

struct queue_node {
	Item item;
	queue_node *next;
};

static queue_node *queue_node_new (Item);

/** Create a new, empty Queue. */
queue *queue_new (void)
{
	queue *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate queue");
	(*new) = (queue) { .n_items = 0, .head = NULL, .tail = NULL };
	return new;
}

/** Destroy a Queue, releasing all resources associated with it. */
void queue_drop (queue *q)
{
	assert (q != NULL);
	for (queue_node *curr = q->head; curr != NULL;) {
		queue_node *next = curr->next;
		free (curr);
		curr = next;
	}
    free (q);
}

/** Add an item to the end of a Queue.
 * Sometimes referred to as "enqueue" or "unshift". */
void queue_en (queue *q, Item it)
{
	queue_node *node = queue_node_new (it);
	if (q->head != NULL) {
		q->tail->next = node;
	} else {
		q->head = node;
	}

	q->tail = node;
	q->n_items++;
}

/** Remove an item from the front of a Queue.
 * Sometimes referred to as "dequeue" or "shift". */
Item queue_de (queue *q)
{
	assert (q != NULL);

	if (q->n_items == 0) {
		fprintf (stderr, "queue underflow!\n");
		abort();
	}
	Item it = q->head->item;
	queue_node *del = q->head;
	q->head = q->head->next;
	free (del);
	q->n_items--;
	return it;
}

/** Get the number of items in a Queue. */
size_t queue_size (queue *q)
{
	assert (q != NULL);
	return q->n_items;
}

static queue_node *queue_node_new (Item it)
{
	queue_node *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate queue_node");
	(*new) = (queue_node) { .item = it, .next = NULL };
	return new;
}

void white_box_tests (void)
{
	// Create queue
	queue *q = queue_new();
	assert(q);
	assert(q->n_items == 0);
	assert(q->head == NULL);
	assert(q->tail == NULL);

	// Enqueue
	queue_en(q, 5);
	assert(q->n_items == 1);
	assert(q->head == q->tail);
	assert(q->head->item == 5);

	queue_en(q, 2);
	assert(q->n_items == 2);
	assert(q->head->next == q->tail);
	assert(q->head->item == 5);
	assert(q->tail->item == 2);

	queue_en(q, 0);
	assert(q->n_items == 3);
	assert(q->head->next->next == q->tail);
	assert(q->head->item == 5);
	assert(q->head->next->item == 2);
	assert(q->tail->item == 0);

	assert(queue_de(q) == 5);
	assert(q->head->item == 2);
	assert(q->head->next->item == 0);
	assert(q->tail->item == 0);

	assert(queue_de(q) == 2);
	assert(q->head == q->tail);
	assert(q->head->item == 0);

	queue_drop(q);
}
