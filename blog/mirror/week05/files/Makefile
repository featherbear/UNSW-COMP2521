# COMP2521 19T0 ... lab 4 makefile

CC	 = 2521 3c

all:	travel crawl ts tq tk tg

travel:		travel.o Graph.o Queue.o
travel.o:	travel.c Graph.h
Graph.o:	Graph.c Graph.h
Queue.o:	Queue.c Queue.h Item.h

crawl:		crawl.o url_file.o html.o set.o queue.o directedGraph.o stack.o
crawl:	CFLAGS	 += $(shell pkg-config --cflags libcurl)
crawl:	LDFLAGS	 += $(shell pkg-config --libs libcurl)

url_file.o:	url_file.c url_file.h
html.o:		html.c html.h

set.o:		set.c set.h
ts:		ts.o set.o
ts.o:		ts.c set.h

queue.o:	queue.c queue.h
tq:		tq.o queue.o
tq.o:		tq.c queue.h

stack.o:	stack.c stack.h
tk:		tk.o stack.o
tk.o:		tk.c stack.h

directedGraph.o:directedGraph.c directedGraph.h
tg:		tg.o directedGraph.o
tg.o:		tg.c directedGraph.h

.PHONY: clean
clean:
	rm -f travel crawl ts tq tk tg *.o
