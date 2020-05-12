# COMP2521 19T0 ... assignment 1

CC	 = 2521 3c

.PHONY: all
all:	test_textbuffer

textbuffer.o:	textbuffer.c textbuffer.h

test_textbuffer:	test_textbuffer.o textbuffer.o
test_textbuffer.o:	test_textbuffer.c textbuffer.h

.PHONY: clean
clean:
	-rm -f test_textbuffer test_textbuffer.o
	-rm -f textbuffer.o

.PHONY: give
give:
	give cs2521 assign1 textbuffer.c test_textbuffer.c
