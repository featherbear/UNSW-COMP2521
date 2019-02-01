////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include "game.h"
#include "hunter.h"
#include "hunter_view.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "_structures.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#endif

// GLOBALS
location_t *draculaResolvedHistory;
size_t *draculaResolvedHistory_size;
Map m;
//

static location_t getLastDracLocation(HunterView hv, ssize_t *distance);

// Get the last known (resolved) location of Dracula

// Run-once
static location_t getLastDracLocation(HunterView hv, ssize_t *distance) {
    *distance = -1;

    draculaResolvedHistory = hv_get_all_history(hv, PLAYER_DRACULA, draculaResolvedHistory_size, true);

    location_t lastLocation = UNKNOWN_LOCATION;

    if (*draculaResolvedHistory_size) {
        size_t i;
        for (i = 0; i < *draculaResolvedHistory_size; i++) {
            if (valid_location_p(draculaResolvedHistory[i])) {
                *distance = (ssize_t) i;
                lastLocation = draculaResolvedHistory[i];
                break;
            }
        }
    }

    return lastLocation;
}
//
//static bool draculaTrailKnown(HunterView hv) {
//    location_t trail[TRAIL_SIZE];
//    hv_get_trail(hv, PLAYER_DRACULA, trail);
//    for (size_t i = 0; i < TRAIL_SIZE; i++) if (valid_location_p(trail[i])) return true;
//    return false;
//}

// Find fastest route to a certain location
#include "_queue.h"
#include "map.h"

size_t distanceTo(location_t from, location_t to) {

}

#include "_connections.h"

location_t *
connectionsFrom(HunterView hv, size_t *n_locations, location_t from, enum player player, round_t round, bool road,
                bool rail, bool sea) {

    assert(valid_location_p(from));
    Queue validMoves = queue_new();
    location_t *loc;

    // Get all the connections: {ROAD, RAIL, SEA}
    if (road) {
        Queue road_moves = connections_get_roadways(gv, from, player, m);
        queue_append_unique(validMoves, road_moves);
    }

    if (rail) {
        assert(player != PLAYER_DRACULA);
        Queue rail_moves = connections_get_railways(from, player, m, round);
        queue_append_unique(validMoves, rail_moves);
    }

    if (sea) {
        Queue sea_moves = connections_get_seaways(gv, from, player, m);
        queue_append_unique(validMoves, sea_moves);
    }

    // Consider extra moves
    Queue extra_moves = connections_get_extras(gv, from, player);
    queue_append_unique(validMoves, extra_moves);

    // If dracula doesn't have moves, he must teleport back to Castle Dracula
    size_t queueSize = queue_size(validMoves);

    if (player == PLAYER_DRACULA && queueSize == 0) {
        loc = malloc(1 * sizeof(location_t));
        loc[0] = TELEPORT;
        *n_locations = 1;

        // Put everything in the queue into the array
    } else {
        loc = malloc(queueSize * sizeof(location_t));
        for (size_t i = 0; i < queueSize; i++) loc[i] = (location_t) (size_t) queue_de(validMoves);
        *n_locations = queueSize;
    }

    // Memory Management
    queue_drop(validMoves);

    return loc;
}


location_t **getRoutes(location_t from, location_t, enum player player, size_t *nRoutes) {
    *nRoutes = 0;
    location_t **routes = NULL;

    int shortestDistance = -1;

    size_t nPossibleLocations = 0;
    location_t * startingPoints = connectionsFrom(hv, &nPossibleLocations, player, round, true, true, true)


    routes = realloc(routes, ++(*nRoutes) * sizeof(*location_t));
    routes[nRoutes - 1] = route;

    return routes;


}

location_t fastestRoute(location_t from, location_t to, round_t round, location_t avoid[], size_t avoid_size) {
    if (from == to) {
        printf("Tried to get from %d to %d, but was already there\n", from, to);
        return from;
    }

    if (avoid) {

        // Avoidance -> low priority

        for (int a = 0; a < avoid_size; a++) {
            // avoid[a];
        }

    }


    size_t nPossibleLocations;
    *possibleLocations = hv_get_dests_player_round(hv, &nPossibleLocations, player, round, true, true, true);

    for (size_t n = 0; n < nPossibleLocations; n++) {
        size_t mPossibleLocations;
        location_t *locations = hv_get_dests_player(hv, &mPossibleLocations, player, round + 1, true, true, true);

    }
//    Map m = map_new();
//
//    // BFS Implementation
//
//    bool seen[NUM_MAP_LOCATIONS] = {false};
//
//    int prev[NUM_MAP_LOCATIONS];
//    for (int i = 0; i < NUM_MAP_LOCATIONS; i++) prev[i] = -1;
//
//    Queue q = queue_new();
//    queue_en(q, from);
//
//    printf("Going from %d to %d\n", from, to);
//
//    while (queue_size(q) > 0) {
//        location_t item = queue_de(q);
//        seen[item] = true;
//
//        if (item == to) {
//            puts("  FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND");
//            break;
//        }
//
//        for (map_adj *curr = m->connections[item]; curr; curr = curr->next) {
//            if (!seen[curr->v]) {
//                prev[curr->v] = item;
//                queue_en(q, curr->v);
//            }
//        }
//    }
//
//    queue_drop(q);
//    map_drop(m);
//
//    puts("  Resolve now");
//    if (prev[to] == -1) {
//        printf("Did not find connection");
//        return NOWHERE;
//    }
//
//    location_t resolve = prev[to];
//    while (resolve) {
//        printf("  Resolve %d\n", resolve);
//        if (prev[resolve] == from) return prev[resolve];
//        resolve = prev[resolve];
//    }
//    return NOWHERE;
}

/*



//     //


//     //
// }

*/

void ceebsToFreeMemoryOnEveryCodeExitSoRunThisInstead(HunterView hv, location_t **possibleLocations) {
    enum player player = hv_get_player(hv);
    round_t round = hv_get_round(hv);
    location_t location = hv_get_location(hv, player);

    // First round, spawn hunters in regions
    if (round == 0) {
        switch (player) {
            case PLAYER_LORD_GODALMING:
                register_best_play("PL", "Spawn");
                return;
            case PLAYER_DR_SEWARD:
                register_best_play("SN", "Spawn");
                return;
            case PLAYER_VAN_HELSING:
                register_best_play("LI", "Spawn");
                return;
            case PLAYER_MINA_HARKER:
                register_best_play("SZ", "Spawn");
                return;
        }
    }

    ssize_t lastDracSeen; // last seen `n` moves ago
    location_t lastDracLocation = getLastDracLocation(hv, &lastDracSeen);

    printf("\nlastDracSeen for player %d is: %d\nCurrent loc is %s (%s)\nCurrent round is: %d\n", player, lastDracSeen,
           location_get_abbrev(location), location_get_name(location), round);

    if (lastDracSeen == -1 && (round == 6 || round == 7)) {
        // Perform collaborative research if Dracula's move history not known
        register_best_play(location_get_abbrev(location), "RESEARCH + REST");
        return;
    }

    // TODO ???
//    if (hv_get_health(hv, player) > 2 && lastDracSeen == 0) {
//        register_best_play(location_get_abbrev(lastDracLocation), "Go for the sui-kill");
//        return;
//    }

    if (hv_get_health(hv, PLAYER_DRACULA) <= 15) {
        // Dracula needs to go back to CD, unless he's really far away then stake him rather than camp
        register_best_play(location_get_abbrev(fastestRoute(location, CASTLE_DRACULA)), "To Dracula's Spawn");
        return;
    }

    if (hv_get_health(hv, player) <= 5) {
        register_best_play(location_get_abbrev(location), "REST");
        return;
    }

    // Do travel
    if (lastDracSeen > -1) {
        // Dracula was spotted - start pursuing
        // Depending on how far away we are, and Drac's last position
        // Case 0 - move to where he last was
        // case 1 - pick a destination that is one step away
        // case 2 - pick a destination that is two step away
        // case 3 - pick a destination that is three step away
        // case 4 - pick a destination that is four step away
        // case 5 - pick a destination that is five step away
        // case 6 - pick a destination that is six step away
        register_best_play(location_get_abbrev(fastestRoute(location, lastDracLocation)), "Enroute");
        return;

    }

    // Do random
    size_t nPossibleLocations;
    *possibleLocations = hv_get_dests_player(hv, &nPossibleLocations, player, true, true, true);

    srand(time(NULL));
    register_best_play(location_get_abbrev((*possibleLocations)[(size_t) rand() % nPossibleLocations]), "rng");

}

void decide_hunter_move(HunterView hv) {
    void *possibleLocations = NULL;

    m = map_new();

    ceebsToFreeMemoryOnEveryCodeExitSoRunThisInstead(hv, (location_t **) &possibleLocations);

    map_drop(m);

    if (possibleLocations) free(possibleLocations);
    if (draculaResolvedHistory) free(draculaResolvedHistory);

}
