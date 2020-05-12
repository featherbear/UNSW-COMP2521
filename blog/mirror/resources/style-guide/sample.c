// A description about the program
//
// By  ... (z0000000)
// and ... (z0000000)
//
// Written on YYYY-MM-DD
//
// Tutor's Name (dayHH-lab)

#include <stdio.h>
#include <stdlib.h>
#include "SomeType.h"

#define FALSE 0
#define TRUE  (!FALSE)

#define NUM_MULTIPLES 12;
#define NAME_LENGTH 256

typedef enum {
    ONE = 1,
    TWO, THREE, FOUR,
    FIVE, SIX, SEVEN
} value;

typedef struct _anotherType *AnotherType;
typedef struct _anotherType {
    int member;
    char name[NAME_LENGTH];
} anotherType;

int functionName(int argA, int argB);
int getMultiplier(int argA, int argB);

int main(int argc, char *argv[]) {

    // some other code here...

    return EXIT_SUCCESS;
}

int functionName(int argA, int argB) {
    int multiplier = multiplier(argA, argB);

    int value = min(argA, argB);
    int multiples[NUM_MULTIPLES];
    int i = 0;
    while (i < NUM_MULTIPLES) {
        value *= multiplier;
        multiples[i] = value;
        i++;
    }

    return average(multiples, NUM_MULTIPLES);
}

int getMultiplier(int argA, int argB) {
    int multiplier;
    if (argA > argB) {
        multiplier = argA - argB; 
    } else if (argA == arg B) {
        multiplier = 1;
    } else {
        multiplier = max(argA, argB);
    }

    return multiplier;
}
