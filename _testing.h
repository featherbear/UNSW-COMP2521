//
// Created by Andrew on 11/01/2019.
//

#ifndef ASS02_TESTING_H
#define ASS02_TESTING_H

#define TEST(ast)          A(#ast); assert(ast); O();
#define TEST_MSG(msg, ast) A(msg);  assert(ast); O();

#define O() printf(" [OK]\n");
#define T(m) printf("\n"); printf(m); printf(" ::\n");
#define A(m) printf("  "); printf(m);


#endif //ASS02_TESTING_H
