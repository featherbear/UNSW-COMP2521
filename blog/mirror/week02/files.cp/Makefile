# COMP2521 19T0 ... lab 2 makefile

CC	 = 2521 3c

.PHONY: all
all: test_stack test_queue_array test_queue_list

test_stack:		test_stack.o stack_array.o
test_queue_array:	test_queue.o queue_array.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_queue_list:	test_queue.o queue_list.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

test_stack.o:	test_stack.c stack.h item.h
stack_array.o:	stack_array.c stack.h item.h
test_queue.o:	test_queue.c queue.h item.h
queue_array.o:	queue_array.c queue.h item.h
queue_list.o:	queue_list.c queue.h item.h

.PHONY: clean
clean:
	-rm -f test_stack test_queue_array test_queue_list
	-rm -f test_stack.o test_queue.o
	-rm -f queue_list.o queue_array.o stack_array.o

.PHONY: give
give:
	give cs2521 lab02 \
		stack_array.c \
		stack_test.c \
		queue_array.c \
		queue_list.c \
		queue_test.c
