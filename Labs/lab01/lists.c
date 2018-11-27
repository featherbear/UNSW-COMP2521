// COMP2521 19T0 ... lab 01: a linked list implementation
//
// 2521 3c +leak test_lists.c lists.c -o test_lists

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

// Remove `__unused' tags from your functions before flight.
#define __unused __attribute__((unused))

#include "lists.h"

/** Traverses and prints the list. */
void list_print(link list) {
    link curr = list;
    while (curr != NULL) {
        printf("[%d]->", curr->item);
        curr = curr->next;
    }
    puts("|");
}

/** Create a new node, initialised with the item provided.
 * Return a pointer to node (link) */
link node_new(Item it) {
    link node = malloc(sizeof((*node)));
    if (node == NULL) return NULL;

    node->item = it;
    node->next = NULL;
    return node;
}

/** Insert a new node into a given non-empty list.
 * The node is inserted directly after the head of the list. */
void list_insert_next(link list, link node) {
//	link ptr = list;
//	while(ptr->next) ptr = ptr->next;
//
//	ptr->next = node;

    link head = list;
    link next = head->next;
    head->next = node;
    node->next = next;
}

/** Return the sum of all items in list */
int list_sum_items(link list) {
    int count = 0;
    for (link ptr = list; ptr; ptr = ptr->next) {
        count += ptr->item;
    }
    return count;
}

/** Frees all memory used in the list */
void list_drop(link list) {
    link head = list;
    link ptr = list;

    while (ptr) {
        link next = ptr->next;
        free(ptr);
        ptr = next;

        if (ptr == head) return;
    }


}


/** Create a circular list with the specified number of nodes,
 * with each link storing data from 1 to the number of nodes. */
link clist_new(int n_nodes) {
    link head = NULL;
    link prev = NULL;

    for (int i = 0; i < n_nodes; i++) {
        link node = node_new(i + 1);
        if (i > 0) {
            prev->next = node;
        } else {
            head = node;
        }
        prev = node;
    }

    if (prev) prev->next = head;

    return head;
}

/** print the data in a circular fashion starting from any node */
void clist_print(link clist) {
    link start = clist;
    link curr = start;
    int firstElem = 1;
    for (; curr && (firstElem || curr != start); curr = curr->next) {
        firstElem = 0;
        printf("[%d]->", curr->item);
    }
    puts("start");
}

dlink new_dnode(Item item);

dlink new_dnode(Item item) {
    // I'm meant to create this myself, right?
    dlink node = malloc(sizeof(*node));
    node->next = NULL;
    node->prev = NULL;
    node->item = item;
    return node;
}

/** Create a double-linked list which contains the same values,
 * in the same order as 'list' */
dlink dlist_new_from_list(link list) {
    link listCurr = list;

    dlink head = NULL;
    dlink prev = NULL;
    // Iterate list until NULL
    while (listCurr) {
        dlink node = new_dnode(listCurr->item);
        if (head == NULL) {
            head = node;
        } else {
            prev->next = node;
            node->prev = prev;
        }
        prev = node;

        listCurr = listCurr->next;
    }

    if (head) {
        head->prev = prev;
        prev->next = head;
    }
    return head;
}

/** Print a doubly-linked list. */
void dlist_print(dlink list) {
    printf("end->");
    clist_print((link) (void *) list); // print until the head is revisited
}

/** Frees all the memory used in the double-linked list */
void dlist_drop(dlink list) {
    dlink head = list;
    dlink ptr = list;

    while (ptr) {
        dlink next = ptr->next;
        free(ptr);
        ptr = next;

        if (ptr == head) return;
    }

}


