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

#include "dracula.h"
#include "dracula__ai.h"

#include <string.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#endif



//region_t get_region(location_t loc);
region_t get_region(location_t loc) {
    switch (loc) {
        case CASTLE_DRACULA:
        case GALATZ:
        case VARNA:
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
        case PLYMOUTH:
        case LONDON:
        case EDINBURGH:
        case AMSTERDAM:
        case HAMBURG:
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
            return REGION_4;
        default: return -1;
    }
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

    } else {
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

    }
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
int dracula_getSafeRegion(DraculaView dv) {
    int nHunters[NUM_REGIONS];
    for (int i = 0; i < NUM_PLAYERS - 1; i++) nHunters[get_region(dv_get_location(dv, i))]++;

    // Want to prioritise travelling to Castle Dracula
    if (nHunters[0] <= 1) return 1;

    // Else find the region with the least amount of hunters
    int min = 4;
    for (int i = 0; i < NUM_REGIONS; i++) {
        if (nHunters[i] < min) min = nHunters[i];
    }
    return min;
}

/* Determines if a region is safe*/
bool dracula_isSafeRegion(DraculaView dv, region_t currRegion) {
    int counter = 0;
    for (int i = 0; i < NUM_PLAYERS - 1; i++)
        if (get_region(dv_get_location(dv, i)) == currRegion) counter++;
    return (counter < 2) ? true : false;
}


/* Checks the safe moves and moves within the 'safeRegion' */
location_t dracula_getMoveWithinRegion(DraculaView dv, region_t r) {

    location_t location;
    bool flag = false;
    location_t loc;
    bool *dracula_safeLocations = dracula_getSafeMoves(dv);
    for (int i = 0; i < NUM_MAP_LOCATIONS; i++) {
        if (dracula_safeLocations[i] == false) continue;
        if (get_region(i) == r) {
            flag = true;
            return i;

        }
    }

    //////////////////////////////////
    // Preventative measures
    if (flag == false) {
        for (int i = 0; i < NUM_MAP_LOCATIONS; i++)
            if (dracula_safeLocations[i] == true) return i;
    }
    //////////////////////////////////
}

/* Given a safeRegion and a currLocation, figure out how to get to a particular region */
location_t dracula_getMoveTowardsRegion(DraculaView dv, region_t safeRegion) {
    Map m = map_new();
    bool *dracula_safeLocations = dracula_getSafeMoves(dv);
    Queue dracula_moves = queue_new();
    for (int i = 0; i < NUM_MAP_LOCATIONS; i++) if (dracula_safeLocations[i]) queue_en(dracula_moves, i);

    location_t l;
    switch (safeRegion) {
        case REGION_1:
            puts("Getting move towards region 1 (CASTLE_DRACULA)");
            l = dracula_getShortestPath(dv, m, dracula_moves, CASTLE_DRACULA);
            break;
        case REGION_2:
            puts("Getting move towards region 2 (MUNICH)");
            l = dracula_getShortestPath(dv, m, dracula_moves, MUNICH);
            break;
        case REGION_3:
            puts("Getting move towards region 3 (SARAGOSSA)");
            l = dracula_getShortestPath(dv, m, dracula_moves, SARAGOSSA);
            break;
        case REGION_4:
            puts("Getting move towards region 4 (LONDON)");
            l = dracula_getShortestPath(dv, m, dracula_moves, LONDON);
            break;
    }

    map_drop(m);
    queue_drop(dracula_moves);
    free(dracula_safeLocations);
    printf("Concluded %d to get to to region %ld\n", l, safeRegion + 1);
    return l;
}

bool *dracula_getHunterPossibleLocations(DraculaView dv)
{
    bool *hunter_possibleLocations = calloc(NUM_MAP_LOCATIONS, sizeof (int));
    for (enum player i = 0; i < NUM_PLAYERS - 1; i++) {
        size_t s;
        location_t *hunter_connections = dv_get_dests_player(dv, &s, i, true, true, true);
        for (size_t j = 0; j < s; j++) hunter_possibleLocations[hunter_connections[j]] = true;
    }
    return hunter_possibleLocations;
}
bool *dracula_getHunterLocations(DraculaView dv)
{
    bool *hunter_locations = calloc(NUM_MAP_LOCATIONS, sizeof (int));
    for (enum player i = 0; i < NUM_PLAYERS - 1; i++)  hunter_locations[dv_get_location(dv, i)] = true;
    return hunter_locations;
}

// Get possible dracula moves which don't overlap with possible hunter moves
bool *dracula_getSafeMoves(DraculaView dv) {

    size_t size;
    location_t *moves = dv_get_dests(dv, &size, true, true);
    Queue moves_q = queue_convertArray(moves, size);
    bool *hunter_locations = dracula_getHunterLocations(dv);
    bool *hunter_possibleLocations = dracula_getHunterPossibleLocations(dv);
    bool *dracula_safeLocations = calloc (NUM_MAP_LOCATIONS, sizeof (int));

    while (queue_size(moves_q) != 0) {
        int location = queue_de(moves_q);
        char *l = location_get_name(location);
        printf("The location we're checking is %s\n\n", l);

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
        if (hunter_locations[(int)location] == true) {
            printf("==>Hunter is currently at this location, not adding..\n");
            continue;
        }

        // Delete move is hunters can get to it
        if (hunter_possibleLocations[(int)location] == true) {
            printf("==>Hunter can get to this location, not adding..\n");
            continue;
        }

        dracula_safeLocations[location] = true;
        printf("==>Added %s\n", l);
    }

    ///////////////////////////////////////////////
    // Preventative measures
    bool flag = false;
    for (int i = 0; i < NUM_MAP_LOCATIONS; i++) {
        if (dracula_safeLocations[i]) {
            flag = true;
            break;
        }
    }

    if (flag == false) dracula_safeLocations[0] = moves[0];
    ///////////////////////////////////////////////

    queue_drop(moves_q);
    free(moves);
    free(hunter_locations);
    free(hunter_possibleLocations);
    return dracula_safeLocations;
}

/* Find the shortest path to a location
* Uses a minimum spanning tree */
location_t dracula_getShortestPath(DraculaView dv, Map m, Queue dMoves, location_t target) {
    printf("==> Getting shortest path towards %d\n", target);
    bool hasSeen[NUM_MAP_LOCATIONS] = {false};
    location_t from[NUM_MAP_LOCATIONS];
    for (size_t i = 0; i < NUM_MAP_LOCATIONS; i++) from[i] = -1;

    Queue q = queue_new();
    queue_en(q, queue_de(dMoves));

    while (queue_size(dMoves) != 0) {
        int l = queue_de(dMoves);
        for (map_adj *n = m->connections[l]; n != NULL; n = n->next) {
            if (hasSeen[n->v] == false) {

                hasSeen[n->v] = true;
                from[n->v] = l;
                if (n->v == target) break;
            }
        }
    }
    int tmp = (int) target;
    while (from[tmp] != -1) {
        printf("%d", tmp);
        tmp = from[tmp];
    }
    return tmp;

}







