////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Queues!
//
// 2018-11-22	Jashank Jeremy <jashankj@cse.unsw.edu.au>

#include <stdlib.h>
#include "item.h"

#ifndef CS2521__QUEUE_H_
#define CS2521__QUEUE_H_

typedef struct queue *Queue;

/** Create a new, empty Queue. */
Queue queue_new (void);

/** Destroy a Queue, releasing all resources associated with it. */
void queue_drop (Queue);

/** Remove an item from the front of a Queue.
 * Sometimes referred to as "dequeue" or "shift". */
Item queue_de (Queue q);

/** Add an item to the end of a Queue.
 * Sometimes referred to as "enqueue" or "unshift". */
void queue_en (Queue, Item);

/** Get the number of items in a Queue. */
size_t queue_size (Queue);

#endif // !defined(CS2521__QUEUE_H_)
