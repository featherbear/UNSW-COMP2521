////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Singly-linked lists.
//
// 2018-07-11	Jashank Jeremy <jashankj@cse.unsw.edu.au>

#include <stdlib.h>

#include "item.h"

#ifndef CS2521__LIST_H_
#define CS2521__LIST_H_

#ifndef Item
#error	This header needs 'Item' to be defined!
#endif

typedef struct list {
	struct node {
		Item item;
		struct node *next;
	} *head, *tail;
	size_t size;
} list;

list *list_new (void);

#endif // !defined(CS2521__LIST_H_)
