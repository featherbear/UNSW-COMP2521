
########################################################################
# COMP2521 19t0 // The Fury of Dracula // the View
# view/Makefile: build tests for {Game,Hunter,Dracula}View
#
# 2018-12-31	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>

CC	 = 3c
BINS	 = test_game_view test_hunter_view test_dracula_view test_connections

all: ${BINS}

test_game_view:		test_game_view.o game_view.o map.o places.o _queue.o _dlist.o _connections.o
test_game_view.o:	test_game_view.c game_view.h map.h places.h game.h

test_hunter_view:	test_hunter_view.o hunter_view.o game_view.o map.o places.o _queue.o _dlist.o _connections.o
test_hunter_view.o:	test_hunter_view.c hunter_view.h game_view.h map.h places.h game.h

test_dracula_view:	test_dracula_view.o dracula_view.o game_view.o map.o places.o _queue.o _dlist.o _connections.o
test_dracula_view.o:	test_dracula_view.c dracula_view.h game_view.h map.h places.h game.h

test_connections:      test_connections.o game_view.o map.o places.o _queue.o _dlist.o _connections.o
test_connections.o:    test_connections.c game_view.h map.h places.h game.h

_queue.o:       _queue.c _queue.h
_dlist.o:       _dlist.c _dlist.h
_connections.o: _connections.c _connections.h

places.o:	places.c places.h game.h _connections.h
map.o:		map.c map.h places.h game.h _connections.h
game_view.o:	game_view.c game_view.h game.h _connections.h
hunter_view.o:	hunter_view.c hunter_view.h game.h _connections.h
dracula_view.o:	dracula_view.c dracula_view.h game.h _connections.h

.PHONY: clean
clean:
	-rm -f ${BINS} *.o core

.PHONY: prove
prove: test_game_view test_hunter_view test_dracula_view
	prove -f ./test_game_view ./test_hunter_view ./test_dracula_view
