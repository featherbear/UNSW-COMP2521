# COMP2521 19T0 ... lab 1 makefile

CC	 = 2521 3c

test_lists:	test_lists.o lists.o
test_lists.o:	test_lists.c lists.h
lists.o:	lists.c lists.h

.PHONY: clean
clean:
	-rm -f test_lists test_lists.o lists.o

.PHONY: give
give:
	give cs2521 lab01 lists.c
