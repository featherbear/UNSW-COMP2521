# CC = 2521 3c
CC = gcc

BINS = test_quicksort quicksort
all: ${BINS}

test_quicksort: test_quicksort.c Item.c
quicksort: quicksort.c Item.c

.PHONY: clean
clean:
	rm -f *.o ${BINS}

