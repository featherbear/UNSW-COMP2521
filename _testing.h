// Created by a very Hungry Andrew who didn't eat much dinner yesterday
// Good thing we had alot of chocolates yesterday!
// Don't we also have alot of chocolates in the office :0 !!

#ifndef ASS02_TESTING_H
#define ASS02_TESTING_H

#include <stdio.h>

#define TEST(ast)          A(#ast); assert(ast); O();
#define TEST_MSG(msg, ast) A(msg);  assert(ast); O();

#define O()  printf(" [OK]\n");
#define T(m) printf("\n%s ::\n", m);
#define A(m) printf("  %s", m);

//
// Function Declarations
//

bool itemInArray(int needle, int *haystack, size_t n_haystack);
bool arraysEqual(size_t nA, int *A, size_t nB, int *B);

///

struct expectedData {
    round_t round;
    enum player player;
    int score;
    int health[NUM_PLAYERS];
    location_t location[NUM_PLAYERS];
    location_t history[NUM_PLAYERS][TRAIL_SIZE];
    size_t nConnections;
    location_t *connections;
};

///

/* Check if an item (int) is in an array */
bool itemInArray(int needle, int *haystack, size_t n_haystack) {
    for (size_t i = 0; i < n_haystack; i++) {
        if (haystack[i] == needle) return true;
    }
    return false;
}

#include "places.h"
/* Compare two arrays and checks if they have the same items (unordered) */
bool arraysEqual(size_t nA, int *A, size_t nB, int *B) {
    // Early exit - if the sizes do not match
    if (nA != nB) return false;

    for (size_t i = 0; i < nA; i++) {
        if (!itemInArray(A[i], B, nB)) return false;
    }

    return true;
}


#endif // ASS02_TESTING_H
