////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... a generic item type

/**
 * @file	item.h
 * @brief	A generic `Item' type.
 *
 * Our provided interfaces use this type to make them _polymorphic_ --
 * that is, to allow them to manipulate objects of arbitrary type, so
 * long as general implementations for some or all of these methods are
 * provided.
 *
 * One quirk: many implementations of this mechanism file will have
 * function implementations in them, which may seem unusual.  In this
 * case, these functions would all be marked `static', so wouldn't be
 * visible outside that file (and files it is `#include'd in).
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef CS2521__ITEM_H_
#define CS2521__ITEM_H_

typedef struct _item_ *Item;

/** Given an existing `Item', create a "deep copy" of it. */
typedef Item (*item_clone_fp) (Item);

/** Given an `Item', release all resources it requires. */
typedef void (*item_drop_fp) (Item);

/** Given two `Item's, report whether they're equal. */
typedef bool (*item_eq_fp) (Item, Item);

/** Given two `Item's, compare them as for strcmp(3); if they're equal,
 * return zero; if the first is less than the second, return a value
 * less than zero; if the first is greater than the second, return a
 * value greater than zero. */
typedef int (*item_cmp_fp) (Item, Item);

/** Produce a string representation of an `Item'. */
typedef char *(*item_show_fp) (Item);

/** Return the size of an `Item'. */
typedef size_t (*item_size_fp) (Item);

/** For `Item's which are collections of values, return the number of
 * values in that collection. */
typedef size_t (*item_ord_fp) (Item);

/**
 * A generic value.
 *
 * To implement a properly generic type in C is a horrible process.
 * Often, the `void *' type is considered "generic", but it is not by
 * any stretch of the imagination; instead, it is a hole through which
 * any value can be passed.  (And, yes, you _could_ just hope that the
 * values you put in are of the right type, but...)
 *
 * We thus store three values: a `magic' value, to give us a quick
 * indication of whether we have the right type of value; a `vtable',
 * which points at a set of function pointers to implement operations on
 * this value; and `item', a flexible array member
 */
struct _item_ {
	uint64_t magic;
	struct _item_vtable *vtable;
	char item[];
};

struct _item_vtable {
	item_clone_fp clone;
	item_drop_fp drop;
	item_eq_fp eq;
	item_cmp_fp cmp;
	item_show_fp show;
	item_size_fp size;
	item_ord_fp ord;
};

#define item_clone(it) ((it)->vtable->clone ((it)))
#define item_drop(it)  ((it)->vtable->drop ((it)))
#define item_eq(a,b)   ((a)->vtable->eq ((a), (b)))
#define item_cmp(a,b)  ((a)->vtable->cmp ((a), (b)))
#define item_show(it)  ((it)->vtable->show ((it)))
#define item_size(it)  ((it)->vtable->size ((it)))
#define item_ord(it)   ((it)->vtable->ord ((it)))

#endif // !defined(CS2521__ITEM_H_)
