////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2019-01-30 v???  Written by Jennifer

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
            break;
    }

    return 0;
}

/* Da main function */
void decide_dracula_move(DraculaView dv) {
    location_t move = get_dracula_move(dv);
    printf("CONCLUDED: %s\n", location_get_name(move));
    register_best_play(location_get_abbrev(move),"NO.");
}


location_t get_dracula_move(DraculaView dv) {
    bool *hunter_locations = dracula_getHunterLocations(dv);
    bool CDIsSafe = !hunter_locations[CASTLE_DRACULA];
    free(hunter_locations);

    // Starting round.
    region_t safestRegion = dracula_getSafestRegion(dv);
    if (dv_get_round(dv) == 0) {
        if (CDIsSafe) return CASTLE_DRACULA;

        switch (safestRegion) {
            case REGION_1: return BELGRADE;
            case REGION_2: return BERLIN;
            case REGION_3: return BARCELONA;
            case REGION_4: return LONDON;
        };
    }

    location_t currLocation = dv_get_location(dv, PLAYER_DRACULA);
    region_t currRegion = get_region(currLocation);
    printf(">>> Got Dracula's current location (%s) and current region (%d)\n", location_get_name(currLocation), currRegion);

    // When low on health, prioritise CD
    if (dv_get_health(dv, PLAYER_DRACULA) < 15) {
        printf(">>> Decided to fasttrack towards CD since health is %d\n", dv_get_health(dv, PLAYER_DRACULA));
        return dracula_getMoveTowardsRegion(dv, REGION_1);
    }

    // Travel within same region
    if (dracula_isSafeRegion(dv, currRegion)) {
        printf(">>> Decided to stay in the same region \n");
        return dracula_getMoveWithinRegion(dv, currRegion);
    } else {
        // Region is not safe, so get a new region
        printf(">>> Decided to move towards region %d\n", safestRegion);
        return dracula_getMoveTowardsRegion(dv, safestRegion);
    }
}

/* Finds the safest region (least hunters) */
region_t dracula_getSafestRegion(DraculaView dv) {
    size_t nHunters[NUM_REGIONS] = {0};
    for (enum player p = 0; p < NUM_HUNTERS; p++) nHunters[get_region(dv_get_location(dv, p))]++;
    for (region_t i = NUM_REGIONS - 1; i != 0; i--) if (nHunters[i] <= 1) return i;
    return 0; // Compiler keeps complaining.
}

/* Determines if a region is safe*/
bool dracula_isSafeRegion(DraculaView dv, region_t r) {
    int counter = 0;
    for (enum player p = 0; p < NUM_HUNTERS; p++)
        if (get_region(dv_get_location(dv, p)) == r) counter++;
    return counter <= 1;
}


/* Checks the safe moves and moves within the 'safeRegion' */
location_t dracula_getMoveWithinRegion(DraculaView dv, region_t r) {

    location_t l = UNKNOWN_LOCATION;
    if (r == REGION_1 && dv_get_location(dv, PLAYER_DRACULA) != CASTLE_DRACULA) l = dracula_getMoveTowardsRegion(dv, r);
    else {
        Queue safeMoves = dracula_getSafeMoves(dv);
        while (queue_size(safeMoves) != 0) {
            l = queue_de(safeMoves);

            if (queue_size(safeMoves) == 0) break;
            if (get_region(l) == r) break;
        }
        queue_drop(safeMoves);
    }
    return l;
}

/* Given a safeRegion and a currLocation, figure out how to get to a particular region */
location_t dracula_getMoveTowardsRegion(DraculaView dv, region_t safeRegion) {

    location_t target = UNKNOWN_LOCATION;
    switch (safeRegion) {
        case REGION_1: target = CASTLE_DRACULA; break;
        case REGION_2: target = MUNICH; break;
        case REGION_3: target = SARAGOSSA; break;
        case REGION_4: target = LONDON; break;
    };

    // printf(">>> Finding fastest move towards %s\n", location_get_name(target));
    size_t size;
    location_t best_move = UNKNOWN_LOCATION;
    location_t *connections = dv_get_dests(dv, &size, true, false);

    // Check if we're right next to the target
    for (size_t i = 0; i < size; i++) {
        if (connections[i] == target) {
            best_move = target;
            break;
        }
    }
    free(connections);

    // We're not right next to it!
    if (best_move == UNKNOWN_LOCATION) {

        Queue safeMoves = dracula_getSafeMoves(dv);
        Map m = map_new();
        location_t from;
        int min_hops = NUM_MAP_LOCATIONS;
        while (queue_size(safeMoves) != 0) {
            from = queue_de(safeMoves);
            if (from > NUM_MAP_LOCATIONS && queue_size(safeMoves) == 0) best_move = from;
            else {
                int tmp = dracula_getShortestPath(m, from, target);
                if (tmp < min_hops) {
                    min_hops = tmp;
                    best_move = from;
                }
            }
        }
        map_drop(m);
        queue_drop(safeMoves);
    }
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

    Queue dracMoves = queue_new();

    // We use the fact that DB and HI is always the last ones within the array to check this
    if (moves[0] > NUM_MAP_LOCATIONS) {

        // If there's no other moves, moves[0] is probably DOUBLE_BACK_1
        if (size == 1) queue_en(dracMoves, moves[0]);

        // We want to hie so even if hunter steps into location, he can't find us
        else queue_en(dracMoves, moves[size - 1]);

    }

    if (queue_size(dracMoves) == 0) {

        Queue moves_q = queue_convertArray(moves, size);
        bool *hunter_locations = dracula_getHunterLocations(dv);
        bool *hunter_possibleLocations = dracula_getHunterPossibleLocations(dv);

        while (queue_size(moves_q) != 0) {
            int location = queue_de(moves_q);

            // Preventative measures.
            if (queue_size(moves_q) == 0 && queue_size(dracMoves) == 0) {
                // printf("We have no choice but to add %s\n", location_get_name(location));
                queue_en(dracMoves, location);
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
            if (hunter_locations[(int)location] == true) continue;

            // Skip move if hunters can get to it
            if (hunter_possibleLocations[(int)location] == true)  continue;

            queue_en(dracMoves, location);
        }
        queue_drop(moves_q);
        free(hunter_locations);
        free(hunter_possibleLocations);
    }

    free(moves);
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
    while (from[tmp] != src) {
        tmp = from[tmp];
        counter++;
    }
    queue_drop(q);
    return counter;
}







