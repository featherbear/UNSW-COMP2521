////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Vectors.
//
// 2018-07-11	Jashank Jeremy <jashankj@cse.unsw.edu.au>

#include <stdbool.h>
#include <stdlib.h>

#include "item.h"

#ifndef CS2521__VEC_H_
#define CS2521__VEC_H_

typedef struct vec {
	size_t length;
	size_t capacity;
	Item items[];
} vec;

vec *vec_new (void);
vec *vec_new_with_capacity (size_t);
void vec_drop (vec *);

size_t vec_length (const vec *);
size_t vec_capacity (const vec *);
Item vec_get_nth (const vec *, size_t);
bool vec_is_empty (const vec *);

void vec_insert (vec *, size_t, Item);
Item vec_remove (vec *, size_t);

void vec_push (vec *, Item);
Item vec_pop (vec *);

void vec_rotate_left (vec *, size_t);
void vec_rotate_right (vec *, size_t);

#endif // !defined(CS2521__VEC_H_)
