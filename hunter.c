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

static location_t getLastDracLocation(HunterView hv, ssize_t *distance);

// Get the last known location of Dracula
static location_t getLastDracLocation(HunterView hv, ssize_t *distance) {
    *distance = -1;

    size_t nLocations;
    location_t *locations = hv_get_all_history(hv, PLAYER_DRACULA, &nLocations, true);

    location_t lastLocation = UNKNOWN_LOCATION;

    if (nLocations) {
        size_t i;
        for (i = 0; i < nLocations; i++) {
            if (valid_location_p(locations[i])) {
                *distance = (ssize_t) i;
                lastLocation = locations[i];
                break;
            }
        }
    }

    free(locations);
    return lastLocation;
}

// Find fastest route to a certain location
#include "_queue.h"
#include "map.h"

location_t fastestRoute(location_t from, location_t to) {
    Map m = map_new();

    // BFS Implementation

    bool seen[NUM_MAP_LOCATIONS] = {false};

    int prev[NUM_MAP_LOCATIONS];
    for (int i = 0; i < NUM_MAP_LOCATIONS; i++) prev[i] = -1;

    Queue q = queue_new();
    queue_en(q, from);

    printf("Going from %d to %d\n", from, to);

    while (queue_size(q) > 0) {
        location_t item = queue_de(q);
        seen[item] = true;

        if (item == to) {
            puts("  FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND FOUND");
            break;
        }

        for (map_adj *curr = m->connections[item]; curr; curr = curr->next) {
            if (!seen[curr->v]) {
                prev[curr->v] = item;
                queue_en(q, curr->v);
            }
        }
    }

    queue_drop(q);
    map_drop(m);

    puts("  Resolve now");
    if (prev[to] == -1) return NOWHERE;

    location_t resolve = prev[to];
    while (resolve) {
        printf("  Resolve %d\n", resolve);
        if (prev[resolve] == from) return prev[resolve];
        resolve = prev[resolve];
    }
    return NOWHERE;
}

/*



//     //


//     //
// }

*/


void decide_hunter_move(HunterView hv) {
    enum player player = hv_get_player(hv);
    round_t round = hv_get_round(hv);
    location_t location = hv_get_location(hv, player);

    printf("I am at %d\n", location);

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

    register_best_play(location_get_abbrev(fastestRoute(location, CASTLE_DRACULA)), "Enroute");

    return;

    ssize_t lastDracSeen;
    location_t lastDracLocation = getLastDracLocation(hv, &lastDracSeen);

    printf("\nlastDracSeen for player %d is: %d\nCurrent loc is %s (%s)\nCurrent round is: %d\n", player, lastDracSeen,
           location_get_abbrev(location), location_get_name(location), round);
    if (lastDracSeen == -1
        && (round == 6 || round == 7)) {
        // Perform collaborative research
        register_best_play(location_get_abbrev(location), "RESEARCH");
        return;
    }

    if (lastDracSeen > -1) {
        register_best_play(location_get_abbrev(fastestRoute(location, lastDracLocation)), "Enroute");
        // Dracula was spotted - start pursuing

    }

    if (hv_get_health(hv, player) <= 5) {
        register_best_play(location_get_abbrev(location), "REST");
        return;
    }

    if (hv_get_health(hv, PLAYER_DRACULA) <= 15) {
        // Dracula needs to go back to CD, unless he's really far away then stake him rather than camp
//        register_best_play("","");
    }


    size_t nPossibleLocations;
    location_t *possibleLocations = hv_get_dests_player(hv, &nPossibleLocations, player, true, true, true);

    //
    srand(time(NULL));
    register_best_play(location_get_abbrev(possibleLocations[(size_t) rand() % nPossibleLocations]), "rng");

    // hv_get_trail()
    // if an actual location is in the trail, start making our way to the latest location


    free(possibleLocations);

}
