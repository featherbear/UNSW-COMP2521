//
// Created by Andrew on 17/01/2019.
//

#include "_queue.h"
#include <assert.h>
#include <stdlib.h>

typedef struct queue_node *qNode;

struct queue_container {
    qNode head;
    qNode tail;
    size_t size;
};

struct queue_node {
    int item;
    qNode next;
};

static qNode node_new(int item);


static qNode node_new(int item) {
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