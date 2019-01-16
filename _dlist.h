// Created by Andrew on 7/01/2019.
//
//

//#ifndef ASS02_DLIST_H
//#define ASS02_DLIST_H

typedef struct dlist_node *dNode;
typedef struct dlist_container *dList;

struct dlist_container {
    dNode head;
    dNode tail;
    size_t size;
};

struct dlist_node {
    int item;
    dNode prev;
    dNode next;
};

///

dNode dlist_node_new(int item);
void dlist_push(dList list, int item);
dList dlist_new(void);
void dlist_destroy(dList list);

///

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

void dlist_destroy(dList list) {
    for (dNode node = list->head; node;) {
        dNode node_next = node->next;
        free(node);
        node = node_next;
    }
    free(list);
}

//#endif //ASS02_DLIST_H
