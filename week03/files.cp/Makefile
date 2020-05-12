# COMP2521 19T0 ... lab 3 makefile

CC	 = 2521 3c

test_btree:	test_btree.o btree.o queue.cse32.o
test_btree.o:	test_btree.c btree.h item.h item_int.h
btree.o:	btree.c btree.h item.h item_int.h queue.h

.PHONY: clean
clean:
	-rm -f test_btree test_btree.o btree.o

.PHONY: give
give:
	give cs2521 lab03 btree.c test_btree.c
