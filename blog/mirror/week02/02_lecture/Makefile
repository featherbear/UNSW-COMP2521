# COMP2521 19T0 ... lecture 3

CC	 = 2521 3c

#CC	 = gcc
#CFLAGS	 = -O3

.PHONY: all
all: search_linear search_binary factorial bad_fib good_fib

search_linear:	search_linear.o
search_binary:	search_binary.o
factorial:	factorial.o
bad_fib:	bad_fib.o
good_fib:	good_fib.o

.PHONY: clean
clean:
	-rm -f search_linear search_linear.o
	-rm -f search_binary search_binary.o
	-rm -f factorial factorial.o
	-rm -f bad_fib bad_fib.o
	-rm -f good_fib good_fib.o
