////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 ... a generic item type, specialised for integers
//
// 2018-09-28    Jashank Jeremy <jashankj@cse.unsw.edu.au>

/**
 * @file	item_btree_node.h
 * @brief	A generic 'Item' type; specialised for integers.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"
#include "btree.h"

#ifndef CS2521__ITEM_BTREE_NODE_H_
#define CS2521__ITEM_BTREE_NODE_H_

#ifndef __unused
#define __unused __attribute__((unused))
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"

#define _BTREE_NODE_ITEM_MAGIC 0x4e5453420065646f
#define BTreeNode_item(x) (*((BTreeNode *)(&(x)->item)))
#define BTreeNode_item_new(x) (_btree_node_item_new((x)))

#define _ensure_btree_node_item(x)\
	({ \
		assert ((x) != NULL); \
		assert ((x)->magic == _BTREE_NODE_ITEM_MAGIC);	\
	})

static Item _btree_node_item_new (BTreeNode value);
static Item _btree_node_item_clone (Item it);
static void _btree_node_item_drop (Item it);
static bool _btree_node_item_eq (Item a, Item b);
static int _btree_node_item_cmp (Item a, Item b);
static char *_btree_node_item_show (Item it);
static size_t _btree_node_item_size (Item it);
static size_t _btree_node_item_ord (Item it);


const static struct _item_vtable _btree_node_item_vtable = {
	.clone = _btree_node_item_clone,
	.drop  = _btree_node_item_drop,
	.eq    = _btree_node_item_eq,
	.cmp   = _btree_node_item_cmp,
	.show  = _btree_node_item_show,
	.size  = _btree_node_item_size,
	.ord   = _btree_node_item_ord
};


/** Make a new integer `Item'. */
static Item _btree_node_item_new (BTreeNode value)
{
	struct _item_ *new = malloc (sizeof *new + sizeof (BTreeNode));
	*new = (struct _item_){
		.magic  = _BTREE_NODE_ITEM_MAGIC,
		.vtable = &_btree_node_item_vtable,
	};
	*((BTreeNode *)(&new->item)) = value;
	return new;
}

/** Given an existing `Item', create a "deep copy" of it. */
static Item _btree_node_item_clone (Item it)
{
	_ensure_btree_node_item (it);
	return BTreeNode_item_new (BTreeNode_item (it));
}

/** Given an `Item', release all resources it requires. */
static void _btree_node_item_drop (Item it)
{
	_ensure_btree_node_item (it);
	free (it);
}

/** Given two `Item's, report whether they're equal. */
static bool _btree_node_item_eq (Item a, Item b)
{
	_ensure_btree_node_item (a);
	_ensure_btree_node_item (b);
	return BTreeNode_item (a) == BTreeNode_item (b);
}

/** Given two `Item's, compare them as for strcmp(3); if they're equal,
 * return zero; if the first is less than the second, return a value
 * less than zero; if the first is greater than the second, return a
 * value greater than zero. */
static int _btree_node_item_cmp (Item a, Item b)
{
	_ensure_btree_node_item (a);
	_ensure_btree_node_item (b);

	BTreeNode a_ = BTreeNode_item (a);
	BTreeNode b_ = BTreeNode_item (b);

	// Delegate the comparison to the node value.
	return item_cmp (btree_node_value (a_), btree_node_value (b_));
}

/**
 * Produce a string representation of an `Item`.
 */
static char *_btree_node_item_show (Item it)
{
	_ensure_btree_node_item (it);
#if 0
	char *x;
	asprintf (&x, "%d", BTreeNode_item (it));
	return x;
#endif
	return NULL;
}

/**
 * Return the size of an `Item`.
 */
static size_t _btree_node_item_size (Item it)
{
	_ensure_btree_node_item (it);
	return sizeof (BTreeNode);
}

/**
 * For `Item`s which act as collections of values, return the number of
 * values in that collection.
 */
static size_t _btree_node_item_ord (Item it)
{
	_ensure_btree_node_item (it);
	return btree_size (BTreeNode_item (it));
}

#pragma clang diagnostic pop
#endif // !defined(CS2521__ITEM_BTREE_NODE_H_)
