//
// Created by Andrew on 24/01/2019.
//

#ifndef UNSW_COMP2521_TESTING_H
#define UNSW_COMP2521_TESTING_H

#include <stdio.h>
#include <stdbool.h>

#define TEST(ast)          A(#ast); assert(ast); O();
#define TEST_MSG(msg, ast) A(msg);  assert(ast); O();

#define O()  printf(" [OK]\n");
#define T(m) printf("\n%s ::\n", m);
#define A(m) printf("  %s", m);
#endif //UNSW_COMP2521_TESTING_H
