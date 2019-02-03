//
// Created by Andrew on 7/01/2019.
//

#include "_dlist.h"
#include <assert.h>
#include <stdlib.h>

/* Creates a new dlist */
dList dlist_new(void) {
    dList
    new = malloc(sizeof(*new));
    (*new) = (struct dlist_container) {
            .size = 0,
            .head = NULL,
            .tail = NULL
    };
    return new;
}

/* Creates a new dlist node */
dNode dlist_node_new(int item) {
    dNode
    new = malloc(sizeof(*new));
    assert(new != NULL);
    (*new) = (struct dlist_node) {
            .prev = NULL,
            .next = NULL,
            .item = item,
    };
    return new;
}

/* Adds a new item to the lsit */
void dlist_push(dList list, int item) {
    dNode *headPtr = &(list->head);
    dNode *tailPtr = &(list->tail);

    dNode
    new = dlist_node_new(item);

    if (!*headPtr) *headPtr = new;

    if (*tailPtr) (*tailPtr)->next = new;
    new->prev = *tailPtr;
    *tailPtr = new;

    list->size++;
}

/* Deltes the list */
void dlist_destroy(dList list) {
    for (dNode node = list->head; node;) {
        dNode node_next = node->next;
        free(node);
        node = node_next;
    }
    free(list);
}

