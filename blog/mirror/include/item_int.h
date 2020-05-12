////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... a generic item type, specialised for integers
//
// 2018-09-28    Jashank Jeremy <jashankj@cse.unsw.edu.au>

/**
 * @file	item_int.h
 * @brief	A generic 'Item' type; specialised for integers.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"

#ifndef CS2521__ITEM_INT_H_
#define CS2521__ITEM_INT_H_

#ifndef __unused
#define __unused __attribute__((unused))
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"

#define _INT_ITEM_MAGIC 0x65746e4900726567
#define int_item(x) (*((int *)(&(x)->item)))
#define int_item_new(x) (_int_item_new((x)))

#define _ensure_int_item(x)\
	({ \
		assert ((x) != NULL); \
		assert ((x)->magic == _INT_ITEM_MAGIC);	\
	})

static Item _int_item_new (int value);
static Item _int_item_clone (Item it);
static void _int_item_drop (Item it);
static bool _int_item_eq (Item a, Item b);
static int _int_item_cmp (Item a, Item b);
static char *_int_item_show (Item it);
static size_t _int_item_size (Item it);
static size_t _int_item_ord (Item it);


const static struct _item_vtable _int_item_vtable = {
	.clone = _int_item_clone,
	.drop  = _int_item_drop,
	.eq    = _int_item_eq,
	.cmp   = _int_item_cmp,
	.show  = _int_item_show,
	.size  = _int_item_size,
	.ord   = _int_item_ord
};


/** Make a new integer `Item'. */
static Item _int_item_new (int value)
{
	struct _item_ *new = malloc (sizeof *new + sizeof (int));
	*new = (struct _item_){
		.magic  = _INT_ITEM_MAGIC,
		.vtable = &_int_item_vtable,
	};
	*((int *)(&new->item)) = value;
	return new;
}

/** Given an existing `Item', create a "deep copy" of it. */
static Item _int_item_clone (Item it)
{
	_ensure_int_item (it);
	return int_item_new (int_item (it));
}

/** Given an `Item', release all resources it requires. */
static void _int_item_drop (Item it)
{
	_ensure_int_item (it);
	free (it);
}

/** Given two `Item's, report whether they're equal. */
static bool _int_item_eq (Item a, Item b)
{
	_ensure_int_item (a);
	_ensure_int_item (b);
	return int_item (a) == int_item (b);
}

/** Given two `Item's, compare them as for strcmp(3); if they're equal,
 * return zero; if the first is less than the second, return a value
 * less than zero; if the first is greater than the second, return a
 * value greater than zero. */
static int _int_item_cmp (Item a, Item b)
{
	_ensure_int_item (a);
	_ensure_int_item (b);
	return int_item (a) - int_item (b);
}

/**
 * Produce a string representation of an `Item`.
 */
static char *_int_item_show (Item it)
{
	_ensure_int_item (it);
	char *x;
	asprintf (&x, "%d", int_item (it));
	return x;
}

/**
 * Return the size of an `Item`.
 */
static size_t _int_item_size (Item it)
{
	_ensure_int_item (it);
	return sizeof (int);
}

/**
 * For `Item`s which act as collections of values, return the number of
 * values in that collection.
 */
static size_t _int_item_ord (Item it)
{
	_ensure_int_item (it);
	return 1;
}

#pragma clang diagnostic pop
#endif // !defined(CS2521__ITEM_INT_H_)
