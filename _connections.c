//
// Created by Jennifer on 12/01/2019
//

#include <assert.h>
#include <stdio.h>

#include "_structures.h"
#include "_queue.h"
#include "_connections.h"


#include "places.h"


#include <string.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

/* Takes in 'non-exact' moves and returns exactly where Dracula is (if not resolved to an unknown location) */
location_t resolveExtraLocations(dNode posNode) {
    assert(posNode);
    location_t pos = posNode->item;
    while (!valid_location_p(pos)) {

        // Find the exact location for DOUBLE_BACK_N
        if (DOUBLE_BACK_1 <= pos && pos <= DOUBLE_BACK_5) {
            for (int i = 0; i <= pos - DOUBLE_BACK_1; i++) {
                posNode = posNode->prev;
            }

            // Find the exact location for HIDE
        } else if (pos == HIDE) {
            posNode = posNode->prev;
        }

        pos = posNode->item;

        // Handle TELEPORT and UNKNOWN cases
        switch (pos) {
            case TELEPORT:
                return CASTLE_DRACULA;
            case CITY_UNKNOWN:
            case SEA_UNKNOWN:
            case UNKNOWN_LOCATION:
                return pos;
            default:
                break;
        }

    }

    return pos;
}


/* Consider extra moves:
 * Hunter: Rest
 * Dracula: HIDE && DOUBLE_BACK_N */
Queue connections_get_extras(GameView gv, location_t l, enum player player) {
    Queue q = queue_new();

    // Hunter: REST
    if (player != PLAYER_DRACULA) {
        queue_en(q, l);
        return q;
    }

    // Dracula: HIDE
    location_t lastLoc = resolveExtraLocations(gv->players[PLAYER_DRACULA].moves->tail);
    if (!location_in_trail(gv, PLAYER_DRACULA, HIDE) && location_get_type(lastLoc) != SEA) {
//        queue_en(q, lastLoc);
        queue_en(q, HIDE);
    }

    // Dracula: DOUBLE_BACK_N
    location_t doubleBacks[] = {DOUBLE_BACK_1, DOUBLE_BACK_2, DOUBLE_BACK_3, DOUBLE_BACK_4, DOUBLE_BACK_5};
    Map m = map_new();

    if (!locations_in_trail(gv, PLAYER_DRACULA, doubleBacks, 5)) {
        // At this point only TP, HI, and exact locations can exist

        dNode doubleBackCursor = gv->players[PLAYER_DRACULA].moves->tail;

        location_t currentLocation = resolveExtraLocations(doubleBackCursor);

        queue_en(q, doubleBacks[0]);
        doubleBackCursor = doubleBackCursor->prev;

        for (int i = 1; i < TRAIL_SIZE - 1 && doubleBackCursor->prev; i++) {
            location_t location = resolveExtraLocations(doubleBackCursor);

            if (connections_is_location_connected(m, currentLocation, location, true, false, true)) queue_en(q, doubleBacks[i]);

            doubleBackCursor = doubleBackCursor->prev;
        }
    }

    map_drop(m);
    return q;
}

/* Check if an ARRAY of locations is within a players trail*/
bool locations_in_trail(GameView gv, enum player player, location_t *loc, size_t nLoc) {

    location_t trail[TRAIL_SIZE];
    gv_get_history(gv, player, trail);

    for (size_t i = 0; i < nLoc; i++) {
        // The oldest move in the trail can be discarded
        for (size_t j = 0; j < TRAIL_SIZE - 1; j++)
            if (loc[i] == trail[j]) return true;
    }
    return false;
}


/* Checks if A SINGLE location is in a player's trail */
bool location_in_trail(GameView gv, enum player player, location_t loc) {

    location_t trail[TRAIL_SIZE];
    gv_get_history(gv, player, trail);

    // The oldest move in the trail can be discarded
    for (size_t i = 0; i < TRAIL_SIZE - 1; i++)
        if (trail[i] == loc) return true;

    return false;
}


/* Finds all locations accessible by road */
Queue connections_get_roadways(GameView gv, location_t l, enum player p, Map m) {
    Queue q = queue_new();

    for (map_adj *tmp = m->connections[l]; tmp != NULL; tmp = tmp->next) {
        if (tmp->type == ROAD) {
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
                    queue_append_unique(q, k);
                    break;

                case 3: // Can move up to three stations
                    k = connections_rail_bfs(tmp->v, m, 2);
                    queue_append_unique(q, k);
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


bool connections_is_location_connected(Map m, location_t locA, location_t locB, bool road, bool rail, bool sea) {
    assert(valid_location_p(locA));
    assert(valid_location_p(locB));

    bool createMap = false;
    if (!m) {
        createMap = true;
        m = map_new();
    }

    bool status = false;
    for (map_adj *tmp = m->connections[locA]; tmp; tmp = tmp->next) {
        if (tmp->v == locB) {
            if ((tmp->type == ROAD && road)
                || (tmp->type == RAIL && rail)
                || (tmp->type == SEA && sea)) {
                status = true;
                break;
            }
        }
    }

    if (createMap) map_drop(m);
    return status;
}