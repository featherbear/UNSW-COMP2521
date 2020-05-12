// COMP2521 19T0 ... lab 01: test a linked list implementation
//
// 2018-11-24	Jashank Jeremy <jashank.jeremy@unsw.edu.au>
// YYYY-mm-dd	Your Name Here <zNNNNNNN@student.unsw.edu.au>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lists.h"

int main (void)
{
	/**
	 * These tests follow the create/mutate/inspect pattern discussed in
	 * lectures: we create a well-known state, we perform a simple
	 * operation, and we verify that the state has changed in ways we
	 * expect, _including_ in those places we expect no change to occur.
	 *
	 * You should, of course, do a better job of carving up your tests
	 * into more neatly contained test cases than I've done here.  My
	 * intent here is to very clearly spell out the rigour needed in
	 * testing even these apparently-simple functions.
	**/

	////////////////////////////////////////////////////////////////////

	puts ("Test 1: `list_print' empty list");
	link list = NULL;
	list_print (list);

	puts ("Test 2: `node_new' and `list_insert_next'");
	link ln1 = node_new (1);
	assert (ln1->item == 1);
	assert (ln1->next == NULL);

	link ln2 = node_new (2);
	list_insert_next (ln1, ln2);
	assert (ln1->item == 1);
	assert (ln1->next == ln2);
	assert (ln2->item == 2);
	assert (ln2->next == NULL);

	link ln3 = node_new (3);
	list_insert_next (ln1, ln3);
	assert (ln1->item == 1);
	assert (ln1->next == ln3);
	assert (ln3->item == 3);
	assert (ln3->next == ln2);
	assert (ln2->item == 2);
	assert (ln2->next == NULL);

	puts ("Test 3: `list_print' "
	      "(expected `[1]->[3]->[2]->|' and sublists)");
	list_print (ln1);
	list_print (ln3);
	list_print (ln2);

	////////////////////////////////////////////////////////////////////

	puts ("Test 4: `list_sum_items'");
	assert (list_sum_items (list) == 0);
	assert (list_sum_items (ln1) == 1 + 3 + 2);

	////////////////////////////////////////////////////////////////////

	puts ("Test 5: `clist_new'");
	link cl1 = clist_new (4);
	assert (cl1 != NULL);
	assert (cl1->item == 1);
	assert (cl1->next != NULL && cl1->next != cl1);
	link cl2 = cl1->next;
	assert (cl2->item == 2);
	assert (cl2->next != NULL);
	assert (cl2->next != cl1);
	assert (cl2->next != cl2);
	link cl3 = cl2->next;
	assert (cl3->item == 3);
	assert (cl3->next != NULL);
	assert (cl3->next != cl1);
	assert (cl3->next != cl2);
	assert (cl3->next != cl3);
	link cl4 = cl3->next;
	assert (cl4->item == 4);
	assert (cl4->next != NULL);
	assert (cl4->next != cl2);
	assert (cl4->next != cl3);
	assert (cl4->next != cl4);
	assert (cl4->next == cl1);

	puts ("Test 6: `clist_print'");
	clist_print (cl1);
	clist_print (cl2);
	clist_print (cl3);
	clist_print (cl4);

	////////////////////////////////////////////////////////////////////

	puts ("Test 7: `dlist_new_from_list' on empty list");
	dlink dl = dlist_new_from_list (list);
	assert (dl == NULL);

	puts ("Test 8: `dlist_new_from_list'");
	dlink dl1 = dlist_new_from_list (ln1);
	dlink dl2, dl3;
	assert (dl1 != NULL);
	assert (dl1->item == 1);
	assert (dl1->prev == NULL);
	assert (dl1->next != NULL && dl1->next != dl1);
	dl3 = dl1->next;
	assert (dl3->item == 3);
	assert (dl3->prev == dl1);
	assert (dl3->next != NULL && dl3->next != dl1);
	dl2 = dl3->next;
	assert (dl2->item == 2);
	assert (dl2->prev == dl3);
	assert (dl2->next == NULL);

	puts ("Test 9: `dlist_print'");
	dlist_print (dl1);
	dlist_print (dl3);
	dlist_print (dl2);

	////////////////////////////////////////////////////////////////////

	puts ("Test 10: `list_drop' on LL and CLL");
	list_drop (ln1);
	list_drop (cl1);
	puts ("Test 11: `dlist_drop'");
	dlist_drop (dl1);

	puts ("\nAll tests passed.  You are awesome!");
	return EXIT_SUCCESS;
}
