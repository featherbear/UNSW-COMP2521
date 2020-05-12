# COMP2521 19T0 ... lab 4 makefile

CC	 = 2521 3c

.PHONY: all
all:	test_heap_min_ordered test_heap_min pl euro conn

test_heap_min_ordered: \
		test_heap_min_ordered.o heap_min_ordered.o
test_heap_min:	test_heap_min.o heap_min.o

heap_min.o:	heap_min.c heap_min.h
heap_min_ordered.o: \
		heap_min_ordered.c heap_min_ordered.h

pl:		pl.o places.o
euro:		euro.o map.o places.o
conn:		conn.o map.o places.o

pl.o:		pl.c places.h
euro.o:		euro.c map.h places.h
conn.o:		conn.c map.h places.h
places.o:	places.c places.h
map.o:		map.c map.h places.h
queue.o:	queue.c queue.h item.h

.PHONY: clean
clean:
	-rm -f test_heap_min_ordered test_heap_min_ordered.o heap_min_ordered.o
	-rm -f test_heap_min test_heap_min.o heap_min.o
	-rm -f pl euro conn pl.o euro.o conn.o places.o map.o queue.o

.PHONY: give
give:
	give cs2521 lab04 heap_min_ordered.c heap_min.c map.c
