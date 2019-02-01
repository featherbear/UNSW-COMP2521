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
#include "map.h"
#include <assert.h>
#include "_connections.h"
#include "_queue.h"
#include "map.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#endif

//

// GLOBALS
static Map m = NULL;
static location_t *draculaResolvedHistory = NULL;
static size_t draculaResolvedHistory_size = 0;
static location_t *possibleLocations = NULL;

//

location_t
fastestRoute(location_t from, location_t to, round_t round, enum player player, location_t avoid[], size_t avoid_size);
static location_t *
connectionsFrom(size_t *n_locations, location_t from, enum player player, round_t round);
static location_t getLastDracLocation(HunterView hv, ssize_t *distance);
static void decide_hunter_move_memWrapper(HunterView hv);

// Get the last known (resolved) location of Dracula
// Run-once
static location_t getLastDracLocation(HunterView hv, ssize_t *distance) {
    *distance = -1;

    draculaResolvedHistory = hv_get_all_history(hv, PLAYER_DRACULA, &draculaResolvedHistory_size, true);

    location_t lastLocation = UNKNOWN_LOCATION;

    if (draculaResolvedHistory_size) {
        size_t i;
        for (i = 0; i < draculaResolvedHistory_size; i++) {
            if (valid_location_p(draculaResolvedHistory[i])) {
                *distance = (ssize_t) i;
                lastLocation = draculaResolvedHistory[i];
                break;
            }
        }
    }

    return lastLocation;
}

location_t *
connectionsFrom(size_t *n_locations, location_t from, enum player player, round_t round) {
    assert(valid_location_p(from));
    Queue validMoves = queue_new();
    location_t *loc;

    Queue road_moves = connections_get_roadways(NULL, from, player, m);
    queue_append_unique(validMoves, road_moves);


    Queue rail_moves = connections_get_railways(from, player, m, round);
    queue_append_unique(validMoves, rail_moves);


    Queue sea_moves = connections_get_seaways(NULL, from, player, m);
    queue_append_unique(validMoves, sea_moves);

    // Consider extra moves
    Queue extra_moves = connections_get_extras(NULL, from, player);
    queue_append_unique(validMoves, extra_moves);

    // If dracula doesn't have moves, he must teleport back to Castle Dracula
    size_t queueSize = queue_size(validMoves);

    loc = malloc(queueSize * sizeof(location_t));
    for (size_t i = 0; i < queueSize; i++) loc[i] = (location_t) (size_t) queue_de(validMoves);
    *n_locations = queueSize;

    // Memory Management
    queue_drop(validMoves);

    return loc;
}


location_t
fastestRoute(location_t from, location_t to, round_t round, enum player player, location_t avoid[], size_t avoid_size) {
    if (from == to) return from;

    bool seen[NUM_MAP_LOCATIONS] = {false};

    int prev[NUM_MAP_LOCATIONS];
    for (int i = 0; i < NUM_MAP_LOCATIONS; i++) prev[i] = UNKNOWN_LOCATION;

    int order[NUM_MAP_LOCATIONS];
    for (int i = 0; i < NUM_MAP_LOCATIONS; i++) prev[i] = -1;

    Queue q = queue_new();
    queue_en(q, from);
    prev[from] = from;
    order[from] = 0;

    // Set the locations to avoid as `true` in the seen arrray
    if (avoid_size) for (size_t i = 0; i < avoid_size; i++) seen[avoid[i]] = true;


    while (queue_size(q) > 0) {
        location_t item = queue_de(q);

        if (item == to) break;

        size_t nPossibleLocations = 0;
        location_t *locs = connectionsFrom(&nPossibleLocations, item, player, round + order[item]);

        for (size_t i = 0; i < nPossibleLocations; i++) {
            if (!seen[locs[i]]) {
                seen[locs[i]] = true;

                prev[locs[i]] = item;
                order[locs[i]] = order[item] + 1;

                queue_en(q, locs[i]);
//                printf("Queuing %s [%s]\n", location_get_abbrev(locs[i]), location_get_name(locs[i]));
            }
        }

        free(locs);
    }

    queue_drop(q);

    if (prev[to] == -1 /* || connection takes too long (??) */) {
        puts("GM> ERROR: Did not find connection");
        if (avoid) return fastestRoute(from, to, round, player, NULL, 0);
        return UNKNOWN_LOCATION;
    }

    location_t resolve = to;
//    printf("GM>  prev[to] = %d = %s\n", resolve, location_get_name(resolve));
    while (resolve != UNKNOWN_LOCATION) {
        if (prev[resolve] == from) break;
        resolve = prev[resolve];
    }

    printf("GM> Going from %d [%s] to %d [%s] via %d [%s]\n", from, location_get_name(from), to, location_get_name(to),
           resolve, location_get_name(resolve));
    return resolve;
}


void decide_hunter_move_memWrapper(HunterView hv) {

    enum player player = hv_get_player(hv);
    round_t round = hv_get_round(hv);
    location_t
            location = hv_get_location(hv, player);

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
                register_best_play("SZ", "Spawn");
                return;
            case PLAYER_MINA_HARKER:
                register_best_play("CD", "Spawn");
                return;
        }
    }

    ssize_t lastDracSeen; // last seen `n` moves ago
    location_t lastDracLocation = getLastDracLocation(hv, &lastDracSeen);

    printf("GM> Player %d is at: %d (%s) | Dracula at: %d (%s) `%d` moves ago\n",
           player, location, location_get_name(location),
           lastDracLocation, location_get_name(lastDracLocation), lastDracSeen);

    if (lastDracSeen == -1 && (round == 6 || round == 7)) {
        // Perform collaborative research if Dracula's move history not known
        register_best_play(location_get_abbrev(location), "RESEARCH + REST");
        return;
    }

    if (lastDracSeen > 10 && round % 13 == 0) {
        register_best_play(location_get_abbrev(location), "RESEARCH + REST");
        return;
    }

    if (draculaResolvedHistory[0] == SEA_UNKNOWN) {
        location_t cityPoints[] = {MARSEILLES, VENICE, HAMBURG, CONSTANTA, SANTANDER};
        register_best_play(location_get_abbrev(cityPoints[rand() % 5]), "Sea Split");
    }

    if (hv_get_health(hv, player) > 4 && lastDracSeen == 0 && location == lastDracLocation) {
        register_best_play(location_get_abbrev(lastDracLocation), "Go for the sui-kill");
        return;
    }

    if (hv_get_health(hv, PLAYER_DRACULA) <= 15) {
        // Dracula needs to go back to CD, unless he's really far away then stake him rather than camp
        register_best_play(location_get_abbrev(fastestRoute(location, CASTLE_DRACULA, round, player, NULL, 0)),
                           "To Dracula's Spawn");
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
        register_best_play(location_get_abbrev(fastestRoute(location, lastDracLocation, round, player, NULL, 0)),
                           "Enroute");
        return;

    }

    // Do random
    size_t nPossibleLocations;
    possibleLocations = hv_get_dests_player(hv, &nPossibleLocations, player, true, true, true);

    register_best_play(location_get_abbrev(possibleLocations[(size_t) rand() % nPossibleLocations]), "RNG");
}

void decide_hunter_move(HunterView hv) {
    srand((unsigned int) time(NULL));

    m = map_new();
    decide_hunter_move_memWrapper(hv);

    map_drop(m);
    if (possibleLocations) free(possibleLocations);
    if (draculaResolvedHistory) free(draculaResolvedHistory);

}
