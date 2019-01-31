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

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#endif


void decide_dracula_move(DraculaView dv) {
    //   location_t move = get_dracula_move(dv);
    //  register_best_play(location_get_abbrev(move), "Ya'll need to calm calm. I'm right here!");
}



region_t get_region(location_t loc);
region_t get_region(location_t loc) {
    switch (loc) {
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
            return -1;
    }

    assert(0);
}



location_t get_dracula_move(DraculaView dv)
{
    region_t safeRegion = dracula_getSafeRegion(dv);
    region_t currRegion;
    location_t currLocation;

    if (dv_get_round(dv) == 0) {
        if (dracula_isHunterLocation(dv, CASTLE_DRACULA)) return CASTLE_DRACULA;
        switch (safeRegion)
        {
            case REGION_2: return BERLIN;
            case REGION_3: return BARCELONA;
            case REGION_4: return LONDON;
        };

    } else {
        currLocation = dv_get_location(dv, PLAYER_DRACULA);
        currRegion = get_region(currLocation);

        if (currLocation == CASTLE_DRACULA &&  dracula_isSafeMove(dv, CASTLE_DRACULA)) {
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
bool dracula_canMove(DraculaView dv, location_t move)
{
    bool flag = false;
    size_t size;
    location_t *connections = dv_get_dests(dv, &size, true, true);
    for (size_t i = 0; i < size; i++) if (connections[i] == move) flag = true;
    free(connections);
    return flag;
}


/* Finds the safest region (least hunters) */
int dracula_getSafeRegion(DraculaView dv)
{
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
bool dracula_isSafeRegion(DraculaView dv, region_t currRegion)
{
    int counter = 0;
    for (int i = 0; i < NUM_PLAYERS - 1; i++)
        if (get_region(dv_get_location(dv, i)) == currRegion) counter++;
    return (counter < 2) ? true : false;
}

/* Finds out whether a move is safe */
bool dracula_isSafeMove(DraculaView dv, location_t l)
{
    bool flag = false;
    location_t *connections;
    for (enum player i = 0; i < NUM_PLAYERS - 1; i++) {
        if (dv_get_location(dv, i) == l) return true;

         // Get the connections for the player
        size_t size;
        connections = dv_get_dests_player(dv, &size, i, true, true, true);
        for (int j = 0; j < size; j++) if (connections[j] == l) flag = true;
    }
    free(connections);
    return flag;
}

/* Checks the safe moves and moves within the 'safeRegion' */
location_t dracula_getMoveWithinRegion(DraculaView dv, region_t r)
{
    int l;
    Queue safeMoves = dracula_getSafeMoves(dv);
    for (int i = 0; i < queue_size(safeMoves); i++) {
        l = queue_de(safeMoves);
        if (r == 1) {
            Map m = map_new();
            int tmp = dracula_getShortestPath(dv, m, safeMoves, CASTLE_DRACULA);
            map_drop(m);
            return tmp;
        }
        if (get_region(l) == r) break;
    }

    // Potential problem, if there's none within the region, it'll just return the last one.. which should be fine
    queue_drop(safeMoves);
    return l;
}

/* Given a safeRegion and a currLocation, figure out how to get to a particular region */
location_t dracula_getMoveTowardsRegion(DraculaView dv, region_t safeRegion)
{
    Map m = map_new();
    Queue safeMoves = dracula_getSafeMoves(dv);

    location_t l;
    switch (safeRegion)
    {
        case REGION_1:
            puts("Getting move towards region 1 (CASTLE_DRACULA)");
            l = dracula_getShortestPath(dv, m, safeMoves, CASTLE_DRACULA); break;
        case REGION_2:
            puts("Getting move towards region 2 (MUNICH)");
            l = dracula_getShortestPath(dv, m, safeMoves, MUNICH); break;
        case REGION_3:
            puts("Getting move towards region 3 (SARAGOSSA)");
            l = dracula_getShortestPath(dv, m, safeMoves, SARAGOSSA); break;
        case REGION_4:
            puts("Getting move towards region 4 (LONDON)");
            l = dracula_getShortestPath(dv, m, safeMoves, LONDON); break;
    }

    map_drop(m);
    queue_drop(safeMoves);
    printf("Concluded %d to get to to region %ld\n", l, safeRegion + 1);
    return l;
}


bool dracula_isHunterPossbleLocation(DraculaView dv, location_t l)
{
    // Go through every player
    for (int i = 0; i < NUM_PLAYERS; i++) {

        // Get the connections for the player
        size_t size;
        location_t *connections = dv_get_dests_player(dv, &size, i, true, true, true);
        for (int j = 0; j < size; j++) if (connections[j] == l) return true;
    }
    return false;
}

/* Checks if hunter is at any of the locations*/
bool dracula_isHunterLocation(DraculaView dv, location_t l)
{
    for (int i = 0; i < NUM_PLAYERS - 1; i++)
        if (dv_get_location(dv, i) == l) return true;
    return false;
}

// Get possible dracula moves which don't overlap with possible hunter moves
Queue dracula_getSafeMoves(DraculaView dv)
{
    size_t size;
    location_t *moves = dv_get_dests(dv, &size, true, true);
    Queue moves_q = queue_convertArray(moves, size);
    Queue dracMoves = queue_new();

    while(queue_size(moves_q) != 0){
        int location = queue_de(moves_q);

        // Making sure that the queue isn't empty
        if (queue_size(moves_q) == 0 && queue_size(dracMoves) == 0) queue_en(dracMoves, location);

         // Delete extra moves (DB && HI)
         switch (location)
         {
            case DOUBLE_BACK_1:
            case DOUBLE_BACK_2:
            case DOUBLE_BACK_3:
            case DOUBLE_BACK_4:
            case DOUBLE_BACK_5:
            case HIDE:
                continue;
         };

        // Delete move if hunter are at it
        if (dracula_isHunterLocation(dv, location)) continue;

        // Delete locations that the hunter can get to in the next move
        if (dracula_isHunterPossbleLocation(dv, location)) continue;

        queue_en(dracMoves, location);
    }
    queue_drop(moves_q);
    free(moves);

    return dracMoves;
}

/* Find the shortest path to a location
* Uses a minimum spanning tree */
location_t dracula_getShortestPath(DraculaView dv, Map m, Queue dMoves, location_t target)
{
    printf("==> Getting shortest path towards %d\n", target);
    bool hasSeen[NUM_MAP_LOCATIONS] = {false};
    location_t from[NUM_MAP_LOCATIONS] = {};
    memset(from, NUM_MAP_LOCATIONS, -1);

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
    int tmp = (int)target;
    while (from[tmp] != -1) {
        printf("%d", tmp);
        tmp = from[tmp];
    }
    return tmp;

}

