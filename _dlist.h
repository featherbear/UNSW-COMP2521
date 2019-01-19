// Created by Andrew on 7/01/2019.
//
//

// Note: Dlist is used to store each player's trails while queues were used for getting connections

#ifndef ASS02_DLIST_H
#define ASS02_DLIST_H


#include <stddef.h>

typedef struct dlist_node *dNode;
struct dlist_node {
    int item;
    dNode prev;
    dNode next;
};

typedef struct dlist_container *dList;

struct dlist_container {
    dNode head;
    dNode tail;
    size_t size;
};

/* Creates a new dList */
dList dlist_new(void);

/* Creates a new dNode */
dNode dlist_node_new(int item);

/* Adds a new item onto the list */
void dlist_push(dList list, int item);

/* Drops a dlist for us */
void dlist_destroy(dList list);




#endif //ASS02_DLIST_H
