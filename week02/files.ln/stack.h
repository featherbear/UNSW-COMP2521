////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Stacks!
//
// 2018-11-22	Jashank Jeremy <jashankj@cse.unsw.edu.au>

#include <stdlib.h>

#ifndef CS2521__STACK_H_
#define CS2521__STACK_H_

#ifndef Item
#error	This header needs 'Item' to be defined!
#endif

typedef struct stack *Stack;

/** Create a new, empty Stack. */
Stack stack_new (void);

/** Destroy a Stack, releasing all resources associated with it. */
void stack_drop (Stack);

/** Remove an item from the top of a Stack. */
Item stack_pop (Stack s);

/** Add an item to the top of a Stack. */
void stack_push (Stack, Item);

/** Get the number of items in a Stack. */
size_t stack_size (Stack);

#endif // !defined(CS2521__STACK_H_)
