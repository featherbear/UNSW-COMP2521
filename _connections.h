// Created by Jennifer on 12/01/2019

#ifndef ASS02_CONNECTIONS_H
#define ASS02_CONNECTIONS_H

#include "game.h"
#include "game_view.h"
#include "_queue.h"

///

void connection_array_insert(location_t *l, location_t value);
size_t connections_sizeof_array(location_t *l);
location_t *connections_get_extras (game_view *gv, location_t l, enum player player);
location_t *connections_get_roadways (game_view *gv, location_t l, enum player p, Map m);
location_t *connections_get_railways (game_view *gv, location_t l, enum player p, Map m);
location_t *connections_rail_bfs(location_t loc, Map m, int depth);
int connections_bfs_process(Queue q, qNode n, bool *hasBeenVisited, Map m);
location_t *connections_get_seaways (game_view *gv, location_t l, enum player p, Map m);

///

/* Given a value, add it to the end of the array
 * Also does memory management */
// TODO Not sure if this actually updates the value of array 'l'
void connection_array_insert(location_t *l, location_t value)
{
    size_t size = connections_sizeof_array(l);
    l = realloc(l, (int)size++ * sizeof(location_t));
    l[(int)size++] = value;
}

/* Finds the size of an array */
size_t connections_sizeof_array(location_t *l)
{
	return sizeof(l) / sizeof(location_t);
}

/* Adds the extra locations (HIDE, DOUBLE_BACK, REST) */
location_t *connections_get_extras (game_view *gv, location_t l, enum player player)
{
    location_t *l = malloc(sizeof(1));
	if (l == NULL) perror("Couldn't allocate new array");

    // Extra moves for dracula
    if (player == PLAYER_DRACULA) {

        if (gv->timers.doubleBack == 0)  connection_array_insert(extras, DOUBLE_BACK);
        if (gv->timers.hide == 0)  connection_array_insert(extras, HIDE);

    // Add rest as a move for hunters
    } else  connection_array_insert(validMoves, REST);

}


/* Finds all locations accessible by road */
// TODO: Update 'node'
Queue *connections_get_roadways (game_view *gv, location_t l, enum player p, Map m)
{
    Queue q = queue_new();

    node *tmp = m->connections[l];
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
// TODO: Update 'node'
location_t *connections_get_railways (game_view *gv, location_t l, enum player p, Map m)
{
    assert(p != PLAYER_DRACULA);

    Queue q = queue_new();

    int sum = (int)gv_get_round(gv) + (int)gv_get_player(gv);
    node *tmp = m->connections[l];
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
				queue_drop(exc1);
				break;

			case 3: // Can move up to three stations
				Queue *exc2 = connections_rail_bfs(tmp->location, m, 2);
				queue_append(q, exc2);
				queue_drop(exc2);
				break;
			};
			connection_array_insert(l, tmp->location);
		}
		tmp = tmp->next;
	}
    return l;
}

/* Helper Function to find possible rail paths using BFS */
location_t *connections_rail_bfs(location_t loc, Map m, int depth)
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

	// Returning array..
    location_t *l = malloc(sizeof(*location_t));
	if (l == NULL) perror("Couldn't allocate new array");

    for (int i = 0; i < MAX_NUM_LOCATIONS; i++)
    	if (hasBeenVisited[i]) connections_array_insert(l, i);
}

/* Helper function for BFS to enqueue items
 * Returns the number of nodes added to the queue */
 // TODO Check that this actually updates the value
int connections_bfs_process(Queue q, qNode n, bool *hasBeenVisited, Map m)
{
	int counter = 0;

	node *tmp = m->connections[n->item]
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
location_t *connections_get_seaways (game_view *gv, location_t l, enum player p, Map m)
{
    location_t *l = malloc(sizeof(1));
	if (l == NULL) perror("Couldn't allocate new array");

	node *tmp = m->connections[n->item];
	while (tmp != NULL) {
		if (tmp->type == BOAT) connections_array_insert(l, )
	}

}


#endif //ASS02_CONNECTIONS_H