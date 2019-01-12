// Created by Jennifer on 12/01/2019

#ifndef ASS02_CONNECTIONS_H
#define ASS02_CONNECTIONS_H

#include "game.h"
#include "game_view.h"
#include "map.c"

#include "_queue.h"

///

Queue connections_get_extras (GameView gv, location_t l, enum player player);
Queue connections_get_roadways (GameView gv, location_t l, enum player p, Map m);
Queue connections_get_railways (GameView gv, location_t l, enum player p, Map m);
Queue connections_rail_bfs(location_t loc, Map m, int depth);
int connections_bfs_process(Queue q, qNode n, bool *hasBeenVisited, Map m);
Queue connections_get_seaways (GameView gv, location_t l, enum player p, Map m);

///


/* Adds the extra locations (HIDE, DOUBLE_BACK, REST) */
Queue connections_get_extras (GameView gv, location_t l, enum player player)
{
	Queue q = queue_new();

	// Extra moves for dracula
	if (player == PLAYER_DRACULA) {
        if (gv->currRound != 0) {
    		if (gv->timers.doubleBack == 0)  {
    		    queue_en(q, DOUBLE_BACK_1);
    		    queue_en(q, DOUBLE_BACK_2);
    		    queue_en(q, DOUBLE_BACK_3);
    		    queue_en(q, DOUBLE_BACK_4);
    		    queue_en(q, DOUBLE_BACK_5);
    		}

    		if (gv->timers.hide == 0)  queue_en(q, HIDE);
        }

		// Add rest as a move for hunters
	} else  queue_en(q, REST);

}


/* Finds all locations accessible by road */
Queue connections_get_roadways (GameView gv, location_t l, enum player p, Map m)
{
	Queue q = queue_new();

	map_adj *tmp = m->connections[l];
	while (tmp != NULL)
	{
		if (tmp->type = ROAD) {

			if (p == PLAYER_DRACULA) {

				// Can't visit the hospital and can't go back on trail (unless you call double_back)
				if (tmp->location == HOSPITAL) continue;

				// Make sure it's not round 0;
				if (gv_get_round(gv) != 0)
					if  (tmp->location == gv->players[p]->tail.location) continue;
			}
			queue_en(q, (int)tmp->location);
		}
		tmp = tmp->next;
	}
	return q;
}


/* Finds all the rail connections possible
 * Returns an array of all possible rail connnections */
Queue connections_get_railways (GameView gv, location_t l, enum player p, Map m)
{
	assert(p != PLAYER_DRACULA);

	Queue q = queue_new();

	int sum = (int)gv_get_round(gv) + (int)gv_get_player(gv);
	map_adj *tmp = m->connections[l];
	while (tmp != NULL)
	{
		if (tmp->type == RAIL) {

			switch (sum % 4)
			{
				case 0: // CANNOT move by rail
					continue;

				case 1: // Can only move one station
					break;

				case 2:  // Can move up to two stations
					Queue *exc1 = connections_rail_bfs(tmp->location, m, 1);
					queue_append(q, exc1);
					break;

				case 3: // Can move up to three stations
					Queue *exc2 = connections_rail_bfs(tmp->location, m, 2);
					queue_append(q, exc2);
					break;
			};
			queue_en(q, (int)tmp->location)
		}
		tmp = tmp->next;
	}
	return l;
}

/* Helper Function to find possible rail paths using BFS */
Queue connections_rail_bfs(location_t loc, Map m, int depth)
{
	// Keep track of what's been visited
	bool hasBeenVisited[MAX_NUM_LOCATIONS] = {0};
	hasBeenVisited[loc] = true;

	Queue q = queue_new();
	int currDepth = 0; // Keeps track of how many stations we can hop
	int countDown = 1; // Counts down the number of elements until we increase the depth
	int countDownNext = 0; //Counts down number of elements (doesn't include curr)

	queue_en(q, loc)
	while (q->size != 0)
	{
		qNode currNode = queue_de(q);
		int nChildren = connections_bfs_process(q, currNode, hasBeenVisited, m);
		countDownNext += nChildren;

		// When there are no more children to check
		if (--countDown == 0) {

			// And we've reached the depth we want
			if (++currDepth > depth) return;
			countDown = countDownNext;
			countDownNext = 0;
		}
	}
	queue_drop(q);

	// Queue to return
	q = queue_new();
	for (int i = 0; i < MAX_NUM_LOCATIONS; i++)
		if (hasBeenVisited[i]) queue_en(q, i);

	return q;
}

/* Helper function for BFS to enqueue items
 * Returns the number of nodes added to the queue */
// TODO Check that this actually updates the value
int connections_bfs_process(Queue q, qNode n, bool *hasBeenVisited, Map m)
{
	int counter = 0;

	map_adj *tmp = m->connections[n->item]
	while (tmp != NULL) {

		if (tmp->type == RAIL && hasBeenVisited[(int)tmp->location]) == false) {

			queue_en(q, (int)tmp->location)

			// Need to update that we have visited it
			hasBeenVisited[(int)tmp->location] = true;
			counter++;

		}
		tmp = tmp->next;
	}
	return counter;
}

/* Finds all sea connections available
 * Returns array of all possible sea/boat connections */
Queue connections_get_seaways (GameView gv, location_t l, enum player p, Map m)
{
	Queue q = queue_new();

	map_adj *tmp = m->connections[n->item];
	while (tmp != NULL) {
		if (tmp->type == BOAT) queue_en(q, (int)tmp->location)
	}
}


#endif //ASS02_CONNECTIONS_H