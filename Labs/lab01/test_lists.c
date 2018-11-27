// COMP2521 19T0 ... lab 01: test a linked list implementation
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lists.h"

typedef int bool;
#define true 1
#define false 0

bool assert_clist(link list);

bool assert_clist(link list) {
    printf("  :: assert_clist\n");

    if (list == NULL) {
        printf("  :: supplied list was empty - aborting\n");
        return false;
    }

    link start = list;
    int startVal = start->item;
    link cursor = start;

    int i = 0;
    while (cursor) {
        printf("     Item %d has the value %d\n", i, cursor->item);
        if (cursor->next == start) {
            int cnext_val = cursor->next->item;
            if (cnext_val == startVal) {
                printf("  :: cursor->next == %p == start | item == %d\n", (void *) cursor->next, cnext_val);
                printf("  :: assert success\n");
                return true;
            }
            return false;
        }
        cursor = cursor->next;
        i++;
    }
    printf("  :: assert FAIL\n");
    return false; // list reached a null
}

bool assert_dlist(dlink list);

bool assert_dlist(dlink list) {
    printf("  :: assert_dlist\n");

    if (list == NULL) {
        printf("  :: supplied list was empty - aborting\n");
        return false;
    }

    dlink start = list;
    int startVal = start->item;
    dlink cursor = start;

    int checks = 0;
    int i = 0;
    while (cursor) {
        printf("     Item %d has the value %d\n", i, cursor->item);
        if (cursor->next == start) {
            int cnext_val = cursor->next->item;
            if (cnext_val == startVal) {
                printf("  :: cursor->next == %p == start | item == %d\n", (void *) cursor->next, cnext_val);
                checks++;
            }
            break;
        }
        cursor = cursor->next;
        i++;
    }

    cursor = start;
    while (cursor) {
        printf("     Item %d has the value %d\n", i, cursor->item);
        if (cursor->prev == start) {
            int cprev_val = cursor->prev->item;
            if (cprev_val == startVal) {
                printf("  :: cursor->prev == %p == start | item == %d\n", (void *) cursor->prev, cprev_val);
                checks++;
            }
            break;
        }
        cursor = cursor->prev;
        i--;
    }
    printf("  :: assert %s\n", checks == 2 ? "success" : "FAIL");
    return checks == 2;
}

int main(void) {
    // Creates an empty list, and then prints it.

    printf("Exercise 1, 2: Linked lists\n");
    link list = node_new(1);                // 1
    list_insert_next(list, node_new(2));    // 1, [2]
    list_insert_next(list, node_new(3));    // 1, [3], 2
    printf("Created list: ");
    list_print(list);
    printf("Sum of this list is: %d\n", list_sum_items(list));
    // Exercise 2
    list_drop(list); // 2521 3c +leak test_lists.c lists.c -o test_lists

    printf("\nExercise 3: Circular linked list\n");
    link clist_0 = clist_new(0);
    printf("0 item circular list: ");
    clist_print(clist_0);
    assert(assert_clist(clist_0) == false);
    list_drop(clist_0);

    link clist_1 = clist_new(1);
    printf("\n1 item circular list: ");
    clist_print(clist_1);
    assert(assert_clist(clist_1)); // link elem should point to itself
    list_drop(clist_1);

    link clist_5 = clist_new(5);
    printf("\n5 item circular list: ");
    clist_print(clist_5);
    assert(assert_clist(clist_5)); // 5th link elem should point to the first
    list_drop(clist_5);

    printf("\nExercise 4: Double linked lists\n");
    list = node_new(1);
    list_insert_next(list, node_new(2));
    list_insert_next(list, node_new(3));
    printf("Input linked list: ");
    list_print(list);

    dlink dlist = dlist_new_from_list(list);
    printf("Double linked list: ");
    dlist_print(dlist);
    assert(assert_dlist(dlist));

    list_drop(list);
    dlist_drop(dlist);

    return EXIT_SUCCESS;
}
