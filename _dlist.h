// Created by Andrew on 7/01/2019.
//
//

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



//typedef struct dlist_node *dNode;
//typedef struct dlist_container *dList;

dNode dlist_node_new(int item);
void dlist_push(dList list, int item);
dList dlist_new(void);
void dlist_destroy(dList list);

#endif //ASS02_DLIST_H
