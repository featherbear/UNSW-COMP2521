// Created by Jennifer on 12/01/2019.
// I used ur template :] Hope you don't mind

#ifndef ASS02_QUEUE_H
#define ASS02_QUEUE_H

#include <assert.h>

typedef struct queue_node *qNode;
typedef struct queue_container *Queue;

struct queue_container {
    qNode head;
    qNode tail;
    size_t size;
};

struct queue_node {
    int item;
    qNode next;
};

//

qNode node_new(int item);
Queue queue_new(void);
void queue_en(Queue q, int item);
void queue_drop(Queue q);
void queue_append(Queue q, Queue p);
int queue_de(Queue q);
size_t queue_size(Queue q);
///

qNode node_new(int item) {
    qNode
    new = malloc(sizeof(*new));
    assert(new != NULL);
    (*new) = (struct queue_node) {
            .next = NULL,
            .item = item,
    };
    return new;
}

Queue queue_new(void) {
    Queue
    new = malloc(sizeof(*new));
    (*new) = (struct queue_container) {
            .size = 0,
            .head = NULL,
            .tail = NULL
    };
    return new;
}

void queue_en(Queue q, int item) {
    assert(q != NULL);
    qNode tail = q->tail;
    tail->next = node_new(item);
    q->tail = tail;
    q->size++;
}

// Returns the value and then frees the node
int queue_de(Queue q) {
    assert(q != NULL);

    qNode old_head = q->head;
    q->head = old_head->next;
    q->size--;

    int tmp = old_head->item;
    free(old_head);
    return tmp;
}

// Appends and drops
void queue_append(Queue q, Queue p) {
    for (size_t i = 0; i < p->size; i++) queue_en(q, queue_de(p));
    queue_drop(p);
}

void queue_drop(Queue q) {
    if (q == NULL) return;
    if (q->size > 0) {

        qNode tmp = q->head;
        for (size_t i = 0; i < q->size; i++) {
            qNode tmp_next = tmp->next;
            free(tmp);
            tmp = tmp_next;
        }
    }
    free(q);
}


// Size of queue
size_t queue_size(Queue q) {
    return q->size;
}

#endif //ASS02_QUEUE_H
