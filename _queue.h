//
// Created by Jennifer on 12/01/2019
//

// Note: Dlist is used to store each player's trails while queues were used for getting connections

#ifndef ASS02_QUEUE_H
#define ASS02_QUEUE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct queue_container *Queue;

/* Create a new queue */
Queue queue_new(void);

/* Enqueue an item */
void queue_en(Queue q, int item);

/* Dequeue an item
 * Returns the value of the node */
int queue_de(Queue q);

/* Appends Queue `p` to Queue `q`
 * Drops Queue `p`
 * Also manages the queue so that there are no repetitions */
void queue_append_unique(Queue q, Queue p);

/* Checks whether a value is already within a given Queue `q` */
bool queue_contains(Queue q, int value);

/* Finds and returns the size of the queue */
size_t queue_size(Queue q);

/* Drops an entire queue */
void queue_drop(Queue q);



#endif //ASS02_QUEUE_H
