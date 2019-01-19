// Created by Jennifer on 12/01/2019

#include <assert.h>
#include <stdio.h>

// #include "game.h"
// #include "game_view.h"
#include "_structures.h"
// #include "map.h"
// #include "places.h"
#include "_queue.h"

#include "_connections.h"


/* Adds the extra locations (HIDE, DOUBLE_BACK, "rest" ) */
Queue connections_get_extras(GameView gv, location_t l, enum player player) {

    Queue q = queue_new();

    // Extra moves for dracula
    if (player == PLAYER_DRACULA) {

        if (!location_in_trail(gv, player, DOUBLE_BACK_1)
            && !location_in_trail(gv, player, DOUBLE_BACK_2)
            && !location_in_trail(gv, player, DOUBLE_BACK_3)
            && !location_in_trail(gv, player, DOUBLE_BACK_4)
            && !location_in_trail(gv, player, DOUBLE_BACK_5)) {

            switch (gv_get_round(gv)) {
                default:
                    queue_en(q, DOUBLE_BACK_5);
                case 5:
                    queue_en(q, DOUBLE_BACK_4);
                case 4:
                    queue_en(q, DOUBLE_BACK_3);
                case 3:
                    queue_en(q, DOUBLE_BACK_2);
                case 2:
                    queue_en(q, DOUBLE_BACK_1);
                case 1:
                case 0:
                    break;
            }
        }


        if (!location_in_trail(gv, player, HIDE) && gv_get_round(gv) != 0 && location_get_type(l) != SEA)
            queue_en(q, HIDE);

    } else queue_en(q, l);

    return q;
}

/* Checks if a location is in a player's trail */
bool location_in_trail(GameView gv, enum player player, location_t loc) {

    location_t trail[TRAIL_SIZE];
    gv_get_history(gv, player, trail);

    // Do an iterative check (OR DID I MEAN iNTErAcTiVe ydwy)
    for (size_t i = 0; i < TRAIL_SIZE; i++)
        if (trail[i] == loc) return true;

    return false;
}

/* Finds all locations accessible by road */
Queue connections_get_roadways(GameView gv, location_t l, enum player p, Map m) {
    Queue q = queue_new();

    for (map_adj *tmp = m->connections[l]; tmp != NULL; tmp = tmp->next) {
        if (tmp->type == ROAD) {
            printf("Road | Looking at %zu\n", tmp->v);
            if (p == PLAYER_DRACULA) {

                // Dracula can't visit the hospital..
                if (tmp->v == HOSPITAL_LOCATION) continue;

                // ..and can't stay in his trail
                if (location_in_trail(gv, p, tmp->v)) continue;

            }
            queue_en(q, (int) tmp->v);
        }
    }

    return q;
}


/* Finds all the rail connections possible */
Queue connections_get_railways(location_t l, enum player p, Map m, round_t round) {
    assert(p != PLAYER_DRACULA);

    Queue q = queue_new();
    Queue k;

    int sum = (int) round + (int) p;

    for (map_adj *tmp = m->connections[l]; tmp; tmp = tmp->next) {
        if (tmp->type == RAIL) {

            switch (sum % 4) {
                case 0: // CANNOT move by rail
                    return q;

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
            queue_en(q, (int) tmp->v);
        }
    }

    return q;
}

/* Helper Function to find possible rail paths using BFS */
Queue connections_rail_bfs(location_t loc, Map m, int depth) {

    // Keep track of what's been visited
    bool hasBeenVisited[NUM_MAP_LOCATIONS] = {false};
    hasBeenVisited[loc] = true;

    Queue q = queue_new();
    int currDepth = 0; // Keeps track of how many stations we have hopped
    int countDown = 1; // Counts down the number of elements until we increase the depth
    int countDownNext = 0; //Counts down number of elements (doesn't include curr)

    queue_en(q, loc);

    while (queue_size(q) != 0) {
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
int connections_bfs_process(Queue q, int item, bool *hasBeenVisited, Map m) {
    int counter = 0;

    for (map_adj *tmp = m->connections[item]; tmp; tmp = tmp->next) {

        if (tmp->type == RAIL && !hasBeenVisited[(int) tmp->v]) {

            queue_en(q, (int) tmp->v);

            // Need to update that we have visited it
            hasBeenVisited[(int) tmp->v] = true;
            counter++;

        }
    }
    return counter;
}

/* Finds all sea connections available */
Queue connections_get_seaways(GameView gv, location_t l, enum player p, Map m) {
    Queue q = queue_new();
    for (map_adj *tmp = m->connections[l]; tmp; tmp = tmp->next) {

        if (tmp->type == BOAT) {
            if (p == PLAYER_DRACULA && location_in_trail(gv, p, tmp->v)) continue;
            queue_en(q, (int) tmp->v);
        }
    }

    return q;
}



