////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2019-01-31 v???  Written by Jennifer

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "dracula.h"
#include "dracula__ai.h"

#define NUM_HUNTERS 4
//
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

region_t get_region(location_t l) {
    switch (l) {
        case CASTLE_DRACULA:
        case GALATZ:
        case VARNA:
        case BUCHAREST:
        case SOFIA:
        case BELGRADE:
        case SZEGED:
        case KLAUSENBURG:
        case CONSTANTA:
        case SALONICA:
        case VALONA:
        case ATHENS:
        case BLACK_SEA:
        case IONIAN_SEA:
            return REGION_1;

        case ST_JOSEPH_AND_ST_MARYS:
        case SARAJEVO:
        case BUDAPEST:
        case VIENNA:
        case ZAGREB:
        case MUNICH:
        case NUREMBURG:
        case FRANKFURT:
        case BERLIN:
        case LEIPZIG:
        case COLOGNE:
        case STRASBOURG:
        case ZURICH:
        case GENEVA:
        case MILAN:
        case BRUSSELS:
        case PARIS:
        case CLERMONT_FERRAND:
        case BARI:
        case NAPLES:
        case ROME:
        case VENICE:
        case FLORENCE:
        case GENOA:
        case MARSEILLES:
        case CAGLIARI:
        case TYRRHENIAN_SEA:
        case ADRIATIC_SEA:
            return REGION_2;

        case MANCHESTER:
        case GALWAY:
        case DUBLIN:
        case SWANSEA:
        case LIVERPOOL:
        case PLYMOUTH:
        case LONDON:
        case EDINBURGH:
        case AMSTERDAM:
        case HAMBURG:
        case PRAGUE:
        case LE_HAVRE:
        case ENGLISH_CHANNEL:
        case NORTH_SEA:
        case IRISH_SEA:
        case ATLANTIC_OCEAN:
            return REGION_3;

        case MADRID:
        case SARAGOSSA:
        case TOULOUSE:
        case GRANADA:
        case NANTES:
        case BORDEAUX:
        case SANTANDER:
        case LISBON:
        case CADIZ:
        case ALICANTE:
        case BARCELONA:
        case MEDITERRANEAN_SEA:
        case BAY_OF_BISCAY:
            return REGION_4;

        default:
            // printf("Couldn't find %s\n", location_get_name(l));
            break;
    }

    return 0;
}

/* Da main function */
void decide_dracula_move(DraculaView dv) {
    location_t move = get_dracula_move(dv);
    register_best_play(location_get_abbrev(move),"NO.");
}


location_t get_dracula_move(DraculaView dv) {
    printf("Deciding dracula's move: \n");
    bool *hunter_locations = dracula_getHunterLocations(dv);
    bool CDIsSafe = !hunter_locations[CASTLE_DRACULA];
    free(hunter_locations);

    // Starting round.
    region_t safestRegion = dracula_getSafestRegion(dv);
    if (dv_get_round(dv) == 0) {
        printf(">>> It is currently round 0, deciding to go to somewhere in region %d\n", safestRegion);
        if (CDIsSafe) return CASTLE_DRACULA;

        switch (safestRegion) {
            case REGION_1:
                return BELGRADE;
            case REGION_2:
                return BERLIN;
            case REGION_3:
                return BARCELONA;
            case REGION_4:
                return LONDON;
        };
    }

    location_t currLocation = dv_get_location(dv, PLAYER_DRACULA);
    region_t currRegion = get_region(currLocation);
    printf(">>> Got Dracula's current location (%s) and current region (%d)\n", location_get_name(currLocation), currRegion);

    // When @CD, want to stay there for as long as possible
    if (currLocation == CASTLE_DRACULA && CDIsSafe) {
        printf(">>> CD is safe\n");
        if (dracula_canMove(dv, DOUBLE_BACK_1)) {
            printf("CONCLUDED: DOUBLE_BACK_1\n");
            return DOUBLE_BACK_1;
        }

        if (dracula_canMove(dv, HIDE)) {
            printf("CONCLUDED: HIDE\n");
            return HIDE;
        }
    }

    // Travel within same region
    if (dracula_isSafeRegion(dv, currRegion)) {
        printf(">>> Decided to stay in the same region \n");
        printf("CONCLUDED: %s\n", location_get_name(dracula_getMoveWithinRegion(dv, currRegion)));
        return dracula_getMoveWithinRegion(dv, currRegion);
    }
    // Region is not safe, so get a new region
    printf(">>> Decided to move towards region %d\n", safestRegion);
    printf("CONCLUDED: %s\n", location_get_name(dracula_getMoveTowardsRegion(dv, currRegion)));
    return dracula_getMoveTowardsRegion(dv, safestRegion);

    // printf("___ERROR___: Code should not reach here.\n");
}

/* Checks if a certain move is possible for Dracula
 * Is used to check DB and HI within dracula.c */
bool dracula_canMove(DraculaView dv, location_t move) {
    bool flag = false;
    size_t size;
    location_t *connections = dv_get_dests(dv, &size, true, true);
    for (size_t i = 0; i < size; i++) if (connections[i] == move) flag = true;
    free(connections);
    return flag;
}


/* Finds the safest region (least hunters) */
region_t dracula_getSafestRegion(DraculaView dv) {
    size_t nHunters[NUM_REGIONS] = {0};
    for (enum player p = 0; p < NUM_HUNTERS; p++) nHunters[get_region(dv_get_location(dv, p))]++;

    // Want to prioritise travelling to Castle Dracula
    if (nHunters[REGION_1] <= 1) return REGION_1;

    // Else find the region with the least amount of hunters
    size_t min = 5;
    for (int i = 0; i < NUM_REGIONS; i++) {
        if (nHunters[i] < min) min = nHunters[i];
    }
    return min;
}

/* Determines if a region is safe*/
bool dracula_isSafeRegion(DraculaView dv, region_t r) {
    int counter = 0;
    for (enum player p = 0; p < NUM_HUNTERS; p++)
        if (get_region(dv_get_location(dv, p)) == r) counter++;
    return counter < 2;
}


/* Checks the safe moves and moves within the 'safeRegion' */
location_t dracula_getMoveWithinRegion(DraculaView dv, region_t r) {

    location_t l = UNKNOWN_LOCATION;
    Queue safeMoves = dracula_getSafeMoves(dv);
    if (r == REGION_1) {
        l = dracula_getMoveTowardsRegion(dv, r);
        printf(">>> In region %zu and getting move towards CASTLE_DRACULA via %s\n", r, location_get_name(l));
    } else {

        while (queue_size(safeMoves) != 0) {
            l = queue_de(safeMoves);

            if (queue_size(safeMoves) == 0) {
                printf(">>> There is only one available move left (getMoveWithinRegion) %s\n", location_get_name(l));
                break;
            }
            if (get_region(l) == r) {
                printf(">>> %s is in region %zu\n", location_get_name(l), r);
                break;
            }
            printf(">>> %s is not in region %zu\n", location_get_name(l), r);
        }
    }
    queue_drop(safeMoves);
    return l;
}

/* Given a safeRegion and a currLocation, figure out how to get to a particular region */
location_t dracula_getMoveTowardsRegion(DraculaView dv, region_t safeRegion) {

    // Early Exit if there's only one available move
    Queue safeMoves = dracula_getSafeMoves(dv);
    if (queue_size(safeMoves) == 1) {
        location_t j = queue_de(safeMoves);
        printf(">>> There's only one safe move available: %s\n", location_get_name(j));
        queue_drop(safeMoves);
        return j;
    }

    location_t target = UNKNOWN_LOCATION;
    switch (safeRegion) {
        case REGION_1:
            target = CASTLE_DRACULA;
            break;
        case REGION_2:
            target = MUNICH;
            break;
        case REGION_3:
            target = SARAGOSSA;
            break;
        case REGION_4:
            target = LONDON;
            break;
    };

    printf(">>> Finding fastest move towards %s\n", location_get_name(target));
    // Find the move that is the fastest (least hops) towards the target location
    location_t from;
    location_t best_move = UNKNOWN_LOCATION;
    int min_hops = NUM_MAP_LOCATIONS;
    Map m = map_new();
    while (queue_size(safeMoves) != 0) {
        from = queue_de(safeMoves);
        int tmp = dracula_getShortestPath(m, from, target);
        printf(">>> Checking to see if %s is the fastest.. it takes %d hops\n", location_get_name(from), tmp);
        if (tmp < min_hops) {
            min_hops = tmp;
            best_move = from;
        }
    }

    map_drop(m);
    queue_drop(safeMoves);
    printf(">>> Concluded %s to get to to region %zu\n", location_get_name(best_move), safeRegion + 1);
    return best_move;
}

/* Using an array to quickly check where the hunters are */
bool *dracula_getHunterPossibleLocations(DraculaView dv) {
    bool *hunter_possibleLocations = calloc(NUM_MAP_LOCATIONS, sizeof(int));
    for (enum player p = 0; p < NUM_HUNTERS; p++) {
        size_t s;
        location_t *hunter_connections = dv_get_dests_player(dv, &s, p, true, true, true);
        for (size_t j = 0; j < s; j++) hunter_possibleLocations[hunter_connections[j]] = true;
        free(hunter_connections);
    }
    return hunter_possibleLocations;
}

/* Using an array to quickly check where the hunters can potentially get to */
bool *dracula_getHunterLocations(DraculaView dv) {
    bool *hunter_locations = calloc(NUM_MAP_LOCATIONS, sizeof(bool));
    for (enum player p = 0; p < NUM_HUNTERS; p++) {
        hunter_locations[dv_get_location(dv, p)] = true;
    }
    return hunter_locations;
}

// Get possible dracula moves which don't overlap with possible hunter moves
Queue dracula_getSafeMoves(DraculaView dv) {
    size_t size;
    location_t *moves = dv_get_dests(dv, &size, true, true);

    //////////////////////////////
    printf("Moves given by DraculaView: [");
    for (size_t i = 0; i < size; i++) printf("%s, ", location_get_name(moves[i]));
    printf("] Which is %d locations\n", size);
    //////////////////////////////

    Queue dracMoves = queue_new();
    // Check if DB or HI is the only move available
    // We use the fact that DB and HI is always the last ones within the array to check this
    if (moves[0] > NUM_MAP_LOCATIONS) {
        printf(">>> The only moves available are DB || HI\n");

        // If there's no other moves, moves[0] is probably DOUBLE_BACK_1
        if (size == 1) {
            printf(">>> There's only one move available (%s)\n", location_get_name(moves[0]));
            queue_en(dracMoves, moves[0]);
        }

        // We want to avoid DOUBLE_BACK_1 --> DB_N, where n != 1
        printf(">>> Enqueuing %s\n", location_get_name(moves[1]));
        queue_en(dracMoves, moves[1]);

         //////////////////////////////
        printf("Queue size of available moves: %d\n", queue_size(dracMoves));
        //////////////////////////////

        return dracMoves;
    }

    Queue moves_q = queue_convertArray(moves, size);
    bool *hunter_locations = dracula_getHunterLocations(dv);
    bool *hunter_possibleLocations = dracula_getHunterPossibleLocations(dv);


    while (queue_size(moves_q) != 0) {
        int location = queue_de(moves_q);

        // Preventative measures.
        if (queue_size(moves_q) == 0 && queue_size(dracMoves) == 0) {
            queue_en(dracMoves, moves[0]);
            break;
        }

        // Delete extra moves (DB && HI)
        switch (location) {
            case DOUBLE_BACK_1:
            case DOUBLE_BACK_2:
            case DOUBLE_BACK_3:
            case DOUBLE_BACK_4:
            case DOUBLE_BACK_5:
            case HIDE:
                continue;
        };

        // Skip move if hunters are at it
        if (hunter_locations[(int) location] == true) {
            // printf("==>Hunter is currently at this location, thank you, next.\n");
            continue;
        }

        // Skip move if hunters can get to it
        if (hunter_possibleLocations[(int) location] == true) {
            // printf("==>Hunter can get to this location, thank you, next.\n");
            continue;
        }

        queue_en(dracMoves, location);
        // printf("==>Added %s\n", l);
    }

    //////////////////////////////
    printf("Queue size of available moves: %d\n", queue_size(dracMoves));
    //////////////////////////////
    queue_drop(moves_q);
    free(moves);
    free(hunter_locations);
    free(hunter_possibleLocations);

    return dracMoves;
}

/* Find the shortest path to a location
 * Uses a minimum spanning tree
 * Returns the number of hops required */
int dracula_getShortestPath(Map m, location_t src, location_t target) {
    bool hasSeen[NUM_MAP_LOCATIONS] = {false};
    location_t from[NUM_MAP_LOCATIONS];
    for (size_t i = 0; i < NUM_MAP_LOCATIONS; i++) from[i] = -1;

    Queue q = queue_new();
    queue_en(q, src);

    // Begin the bfs
    while (queue_size(q) != 0) {
        int l = queue_de(q);
        for (map_adj *n = m->connections[l]; n != NULL; n = n->next) {

            // Check if we've seen the location before
            if (!hasSeen[n->v]) {
                hasSeen[n->v] = true;
                from[n->v] = l;
                if (n->v == target) break;
                queue_en(q, n->v);
            }
        }
    }

    int counter = 0;
    location_t tmp = target;
    // printf("Path for %s is as follows: [", location_get_name(target));
    while (from[tmp] != src) {
        tmp = from[tmp];
        // printf("%s, ", location_get_name(tmp));
        counter++;
    }
    // printf("]\nFrom %s, it takes a total of %d moves to get to %s\n", location_get_name(src), counter, location_get_name(target));
    queue_drop(q);
    return counter;
}







