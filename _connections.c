// Created by Jennifer on 12/01/2019

#include <assert.h>

// #include "game.h"
// #include "game_view.h"
 #include "_structures.h"
// #include "map.h"
// #include "places.h"
 #include "_queue.h"

#include "_connections.h"


/* Adds the extra locations (HIDE, DOUBLE_BACK, REST) */
Queue connections_get_extras (GameView gv, location_t l, enum player player)
{

    Queue q = queue_new();

    // Extra moves for dracula
    if (player == PLAYER_DRACULA) {
        if (gv_get_round(gv) != 0) {
            if (gv->timers.doubleBack == 0)  {
                queue_en(q, DOUBLE_BACK_1);
                queue_en(q, DOUBLE_BACK_2);
                queue_en(q, DOUBLE_BACK_3);
                queue_en(q, DOUBLE_BACK_4);
                queue_en(q, DOUBLE_BACK_5);
            }

            if (gv->timers.hide == 0) queue_en(q, HIDE);
        }

        // Add rest as a move for hunters
    } else queue_en(q, l);

    return q;
}

/* Finds all locations accessible by road */
Queue connections_get_roadways (GameView gv, location_t l, enum player p, Map m)
{
    Queue q = queue_new();

    map_adj *tmp = m->connections[l];
    while (tmp != NULL)
    {
        if (tmp->type == ROAD) {

            if (p == PLAYER_DRACULA) {

                // Can't visit the hospital and can't go back on trail (unless you call double_back)
                if (tmp->v == HOSPITAL_LOCATION) continue;

                // Make sure it's not round 0
                if (gv_get_round(gv) != 0)
                    if  (connections_in_trail(gv, p, tmp->v)) continue;
            }
            queue_en(q, (int)tmp->v);
        }
        tmp = tmp->next;
    }
    return q;
}


/* Finds all the rail connections possible
 * Returns an array of all possible rail connnections */
Queue connections_get_railways (GameView gv, location_t l, enum player p, Map m, round_t round)
{
    assert(p != PLAYER_DRACULA);

    Queue q = queue_new();
    Queue k;

    int sum = (int)round + (int)p;
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
                    k = connections_rail_bfs(tmp->v, m, 1);
                    queue_append(q, k);
                    break;

                case 3: // Can move up to three stations
                    k = connections_rail_bfs(tmp->v, m, 2);
                    queue_append(q, k);
                    break;
            };
            queue_en(q, (int)tmp->v);
        }
        tmp = tmp->next;
    }

    // Note: queue_append handles the queue dropping
    return q;
}

/* Helper Function to find possible rail paths using BFS */
Queue connections_rail_bfs(location_t loc, Map m, int depth)
{
    // Keep track of what's been visited
    bool hasBeenVisited[NUM_MAP_LOCATIONS] = {false};
    hasBeenVisited[loc] = true;

    Queue q = queue_new();
    int currDepth = 0; // Keeps track of how many stations we can hop
    int countDown = 1; // Counts down the number of elements until we increase the depth
    int countDownNext = 0; //Counts down number of elements (doesn't include curr)

    queue_en(q, loc);
    while (q->size != 0)
    {
        int value = queue_de(q);
        int nChildren = connections_bfs_process(q, value, hasBeenVisited, m);
        countDownNext += nChildren;

        // When there are no more children to check
        if (--countDown == 0) {

            // And we've reached the depth we want
            if (++currDepth > depth) break;
            countDown = countDownNext;
            countDownNext = 0;
        }
    }
    queue_drop(q);

    // Queue to return
    q = queue_new();
    for (int i = 0; i < NUM_MAP_LOCATIONS; i++) {
        if (hasBeenVisited[i]) queue_en(q, i);
    }

    return q;
}

/* Helper function for BFS to enqueue items
 * Returns the number of nodes added to the queue */
int connections_bfs_process(Queue q, int item, bool *hasBeenVisited, Map m)
{
    int counter = 0;

    map_adj *tmp = m->connections[item];
    while (tmp != NULL) {

        if (tmp->type == RAIL && hasBeenVisited[(int)tmp->v] == false) {

            queue_en(q, (int)tmp->v);

            // Need to update that we have visited it
            hasBeenVisited[(int)tmp->v] = true;
            counter++;

        }
        tmp = tmp->next;
    }
    return counter;
}

/* Finds all sea connections available
 * Returns array of all possible sea/boat connections */
Queue connections_get_seaways (location_t l, enum player p, Map m)
{
    Queue q = queue_new();

    map_adj *tmp = m->connections[l];
    while (tmp != NULL) {
        if (tmp->type == BOAT) queue_en(q, (int)tmp->v);
    }
    return q;
}


