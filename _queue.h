// Created by Jennifer on 12/01/2019.
// I used ur template :] Hope you don't mind

#ifndef ASS02_QUEUE_H
#define ASS02_QUEUE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct queue_container *Queue;

Queue queue_new(void);
void queue_en(Queue q, int item);
void queue_drop(Queue q);
void queue_append(Queue q, Queue p);
void queue_append_unique(Queue q, Queue p);
int queue_de(Queue q);
size_t queue_size(Queue q);
bool queue_contains(Queue q, int value);

#endif //ASS02_QUEUE_H
