////////////////////////////////////////////////////////////////////////
// COMP2521 19T0 -- Testable implementation interface.
//
// 2018-11-29	Jashank Jeremy <jashankj@cse.unsw.edu.au>

#ifndef CS2521__TESTABLE_H_
#define CS2521__TESTABLE_H_

/**
 * Run black-box tests for this particular interface.
 */
void black_box_tests (void);

/**
 * Run white-box tests for this particular implementation.
 */
void white_box_tests (void);

#endif // !defined(CS2521__TESTABLE_H_)
