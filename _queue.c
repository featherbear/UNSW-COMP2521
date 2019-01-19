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
    qNode new = malloc(sizeof(*new));
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
    printf("    Added location %d\n", item);

}

// Returns the value and then frees the node
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

// Appends and drops
void queue_append(Queue q, Queue p) {
    for (size_t i = 0; i < p->size; i++) {
        int value = queue_de(p);

        if (queue_contains(q, value)) {
            printf("Location: %d is aready seen");
            continue;
        }
        queue_en(q, value);

    }
    queue_drop(p);
}

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


// Size of queue
size_t queue_size(Queue q) {
    return q->size;
}

//

bool queue_contains(Queue q, int value) {
    for (qNode tmp = q->head; tmp != NULL; tmp = tmp->next) if (tmp->item == value) return true;
    return false;
}

