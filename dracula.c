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

#include "dracula.h"
#include "dracula__ai.h"

#include <string.h>

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
            printf("Couldn't find %s\n", location_get_name(l));
            assert(0 && "Unknown Location");
    }

    return 0; // Rip
}

/* Da main function */
void decide_dracula_move(DraculaView dv) {
    location_t move = get_dracula_move(dv);
    register_best_play(location_get_abbrev(move), "Ya'll need to calm calm. I'm right here!");
}


location_t get_dracula_move(DraculaView dv) {
    region_t safeRegion = dracula_getSafeRegion(dv);
    region_t currRegion;
    location_t currLocation;
    bool *hunter_locations = dracula_getHunterLocations(dv);
    bool CDIsSafe = hunter_locations[CASTLE_DRACULA] ? false : true;
    free(hunter_locations);

    if (dv_get_round(dv) == 0) {
        if (CDIsSafe) return CASTLE_DRACULA;
        switch (safeRegion) {
            case REGION_2:
                return BERLIN;
            case REGION_3:
                return BARCELONA;
            case REGION_4:
                return LONDON;
        };
    }

    currLocation = dv_get_location(dv, PLAYER_DRACULA);
    currRegion = get_region(currLocation);

    if (currLocation == CASTLE_DRACULA && CDIsSafe) {
        if (dracula_canMove(dv, DOUBLE_BACK_1)) return DOUBLE_BACK_1;
        if (dracula_canMove(dv, HIDE)) return HIDE;
    }

    // Travel within same region
    if (dracula_isSafeRegion(dv, currRegion)) return dracula_getMoveWithinRegion(dv, currRegion);

    // Region is not safe, so get a new region
    return dracula_getMoveTowardsRegion(dv, safeRegion);
    assert(0);

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
region_t dracula_getSafeRegion(DraculaView dv) {
    size_t nHunters[NUM_REGIONS] = {0};
    for (enum player i = 0; i < NUM_PLAYERS - 1; i++) nHunters[get_region(dv_get_location(dv, i))]++;

    // Want to prioritise travelling to Castle Dracula
    if (nHunters[0] <= 1) return 1;

    // Else find the region with the least amount of hunters
    size_t min = 4;
    for (int i = 0; i < NUM_REGIONS; i++) {
        if (nHunters[i] < min) min = nHunters[i];
    }
    return min;
}

/* Determines if a region is safe*/
bool dracula_isSafeRegion(DraculaView dv, region_t r) {
    int counter = 0;
    for (enum player p = 0; p < NUM_PLAYERS - 1; p++)
        if (get_region(dv_get_location(dv, p)) == r) counter++;

    // printf("There are %d hunters in region %zu\n", counter, r);
    return (counter < 2) ? true : false;
}


/* Checks the safe moves and moves within the 'safeRegion' */
location_t dracula_getMoveWithinRegion(DraculaView dv, region_t r) {

    location_t l = UNKNOWN_LOCATION;
    Queue safeMoves = dracula_getSafeMoves(dv);
    while (queue_size(safeMoves) != 0) {
        l = queue_de(safeMoves);

        // If there's only one item left, return it;
        if (queue_size(safeMoves) == 0) break;
        if (get_region(l) == r) break;
    }

    queue_drop(safeMoves);
    return l;
}

/* Given a safeRegion and a currLocation, figure out how to get to a particular region */
location_t dracula_getMoveTowardsRegion(DraculaView dv, region_t safeRegion) {
    Map m = map_new();
    Queue safeMoves = dracula_getSafeMoves(dv);
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

    // Find the move that is the fastest towards the target location
    location_t from;
    location_t best_move = UNKNOWN_LOCATION;
    int min_hops = NUM_MAP_LOCATIONS;
    while (queue_size(safeMoves) != 0) {
        from = queue_de(safeMoves);
        int tmp = dracula_getShortestPath(m, from, target);
        if (tmp < min_hops) {
            min_hops = tmp;
            best_move = from;
        }
    }

    map_drop(m);
    queue_drop(safeMoves);
    // printf("Concluded %s to get to to region %ld\n", location_get_name(best_move), safeRegion + 1);
    return best_move;
}

/* Using an array to quickly check where the hunters are */
bool *dracula_getHunterPossibleLocations(DraculaView dv) {
    bool *hunter_possibleLocations = calloc(NUM_MAP_LOCATIONS, sizeof(int));
    for (enum player i = 0; i < NUM_PLAYERS - 1; i++) {
        size_t s;
        location_t *hunter_connections = dv_get_dests_player(dv, &s, i, true, true, true);
        for (size_t j = 0; j < s; j++) hunter_possibleLocations[hunter_connections[j]] = true;
        free(hunter_connections);
    }
    return hunter_possibleLocations;
}

/* Using an array to quicky check where the hunters can potentially get to*/
bool *dracula_getHunterLocations(DraculaView dv) {
    bool *hunter_locations = calloc(NUM_MAP_LOCATIONS, sizeof(int));
    for (enum player i = 0; i < NUM_PLAYERS - 1; i++) hunter_locations[dv_get_location(dv, i)] = true;
    return hunter_locations;
}

// Get possible dracula moves which don't overlap with possible hunter moves
Queue dracula_getSafeMoves(DraculaView dv) {

    size_t size;
    location_t *moves = dv_get_dests(dv, &size, true, true);
    Queue moves_q = queue_convertArray(moves, size);
    bool *hunter_locations = dracula_getHunterLocations(dv);
    bool *hunter_possibleLocations = dracula_getHunterPossibleLocations(dv);

    Queue dracMoves = queue_new();

    while (queue_size(moves_q) != 0) {
        int location = queue_de(moves_q);

        // Preventative measures.
        if (queue_size(moves_q) == 0 && queue_size(dracMoves) == 0) {
            queue_en(dracMoves, moves[0]);
            break;
        }

        // Delete extra moves (DB && HI) This function works.
        switch (location) {
            case DOUBLE_BACK_1:
            case DOUBLE_BACK_2:
            case DOUBLE_BACK_3:
            case DOUBLE_BACK_4:
            case DOUBLE_BACK_5:
            case HIDE:
                continue;
        };

        // Delete move if hunters are at it
        if (hunter_locations[(int) location] == true) {
            // printf("==>Hunter is currently at this location, not adding..\n");
            continue;
        }

        // Delete move is hunters can get to it
        if (hunter_possibleLocations[(int) location] == true) {
            // printf("==>Hunter can get to this location, not adding..\n");
            continue;
        }

        queue_en(dracMoves, location);
        // printf("==>Added %s\n", l);
    }

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
            if (hasSeen[n->v] == false) {
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







