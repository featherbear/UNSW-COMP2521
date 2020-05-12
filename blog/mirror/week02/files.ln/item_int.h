////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... a generic item type, specialised for integers
//
// 2018-09-28    Jashank Jeremy <jashankj@cse.unsw.edu.au>

/**
 * @file	item_int.h
 * @brief	A generic 'Item' type; specialised for integers.
 *
 * Our provided interfaces use this type to make them _polymorphic_ --
 * that is, to allow them to manipulate objects of arbitrary type, so
 * long as general implementations for some or all of these methods are
 * provided.
 *
 * One quirk: many implementations of this header file will have
 * function implementations in them, which may seem unusual.  In this
 * case, these functions would all be marked `static`, so wouldn't be
 * visible outside this file (and files we're `#include`d in).  They
 * would also all be marked `inline`, so the compiler attempts to insert
 * the (scoped) bodies of these functions where they're called, reducing
 * the overhead of the function call.
 */

#include <stdbool.h>
#include <stdlib.h>

#ifndef CS2521__ITEM_INT_H_
#define CS2521__ITEM_INT_H_

#ifndef __unused
#define __unused __attribute__((unused))
#endif

typedef int Item;

/**
 * Given an existing `Item`, create a "deep copy" of it.
 */
static inline Item item_clone (Item it) { return it; }

/**
 * Given an `Item`, release all resources it requires.
 */
static inline void item_drop (Item it __attribute__((unused))) { return; }

/**
 * Given two `Item`s, report whether they're equal.
 */
static inline bool item_eq (Item a, Item b) { return a == b; }

/**
 * Given two `Item`s, compare them as for `strcmp(3)`; if they're equal,
 * return zero; if the first is less than the second, return a value
 * less than zero; if the first is greater than the second, return a
 * value greater than zero.
 */
static inline int item_cmp (Item a, Item b) { return b - a; }

/**
 * Produce a string representation of an `Item`.
 */
static inline char *item_show (Item it) {
	char *x; asprintf (&x, "%d", it); return x; }

/**
 * Return the size of an `Item`.
 */
static inline size_t item_size (Item it __unused) { return sizeof(Item); }

/**
 * For `Item`s which act as collections of values, return the number of
 * values in that collection.
 */
static inline int item_ord (Item it __unused) { return 1; }

#define Item Item

#endif // !defined(CS2521__ITEM_INT_H_)

