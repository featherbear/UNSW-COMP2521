// Created by Andrew on 7/01/2019.
//
//

#ifndef ASS02_DLIST_H
#define ASS02_DLIST_H

typedef struct dlist_node *DNode;
typedef struct dlist_container *DList;

typedef struct dlist_container {
    DNode head;
    DNode tail;
    size_t size;
};

struct dlist_node {
    int item;
    DNode prev;
    DNode next;
};

///

DNode dlist_node_new(int item) {
    DNode new = malloc(sizeof(*new));
    assert(new != NULL);
    (*new) = (struct dlist_node) {
            .prev = NULL,
            .next = NULL,
            .item = item,
    };
    return new;
}

void dlist_push(DList list, int item) {
    DNode *headPtr = &(list->head);
    DNode *tailPtr = &(list->tail);

    DNode
    new = dlist_node_new(item);

    if (!*headPtr) *headPtr = new;

    if (*tailPtr) (*tailPtr)->next = new;
    new->prev = *tailPtr;
    *tailPtr = new;

    list->size++;
}

DList dlist_new(void) {
    DList
    new = malloc(sizeof(*new));
    (*new) = (struct dlist_container) {
            .size = 0,
            .head = NULL,
            .tail = NULL
    };
    return new;
}

void dlist_destroy(DList list) {
    for (DNode node = list->head; node;) {
        DNode node_next = node->next;
        free(node);
        node = node_next;
    }
    free(list);
}

#endif //ASS02_DLIST_H
