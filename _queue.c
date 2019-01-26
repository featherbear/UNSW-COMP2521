//
// Created by Jennifer on 12/01/2019.
//

#include "_queue.h"
#include <assert.h>
#include <stdlib.h>

typedef struct queue_node *qNode;

///

struct queue_container {
    qNode head;
    qNode tail;
    size_t size;
};

struct queue_node {
    int item;
    qNode next;
};

///

/* Create a new node */
static qNode node_new(int item);

///


/* Create a new queue */
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

/* Creates a new node */
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

/* Enqueue an item */
void queue_en(Queue q, int item) {
    assert(q != NULL);
    // Check that the queue isn't empty
    if (q->size == 0) {
        q->head = node_new(item);
        q->tail = q->head;
    } else {
        qNode new_tail = node_new(item);
        q->tail->next = new_tail;
        q->tail = new_tail;
    }
    q->size++;
}


/* Dequeue an item
 * Returns the value of the node */
int queue_de(Queue q) {
    assert(q != NULL);
    assert(q->size != 0);

    int tmp_value;
    qNode tmp;
    tmp_value = q->head->item;
    tmp = q->head;
    q->head = q->head->next;
    free(tmp);
    q->size--;
    return tmp_value;
}

/* Appends Queue `p` to Queue `q`
 * Drops Queue `p`
 * Also manages the queue so that there are no repetitions */
void queue_append_unique(Queue q, Queue p) {
    for (qNode tmp = p->head; tmp != NULL; tmp = tmp->next) {
        if (queue_contains(q, tmp->item)) continue;
        queue_en(q, tmp->item);
    }
    queue_drop(p);
}

/* Checks whether a value is already within a given Queue `q` */
bool queue_contains(Queue q, int value) {
    for (qNode tmp = q->head; tmp != NULL; tmp = tmp->next) if (tmp->item == value) return true;
    return false;
}

/* Finds and returns the size of the queue */
size_t queue_size(Queue q) {
    return q->size;
}

/* Drops an entire queue */
void queue_drop(Queue q) {
    if (q == NULL) return;
    if (q->size == 0) {
        free(q);
        return;
    }
    qNode tmp = q->head;
    while (tmp != NULL) {
        qNode tmp_next = tmp->next;
        free(tmp);
        tmp = tmp_next;
    }
    free(q);
}

//

// Check if a value exists within a queue
bool queue_has(Queue q, int value) {
    for (qNode n = q->head; n != NULL; n = n->next) {
        if (n->item == value) return true;
    }
    return false;
}

Queue convert_aTOq(int *a, size_t size) {
    Queue q = queue_new();
    for (size_t i = 0; i < size; i++) queue_en(q, a[i]);
    return q;
}
