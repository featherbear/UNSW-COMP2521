////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include "game.h"
#include "dracula_view.h"
#include "_queue.h"
#include "_ai.h"
#include "map.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#endif

/* Helper functions */
bool dracula_isHunterLocation(DraculaView dv, location_t l);
bool dracula_isHunterPossbleLocation(DraculaView dv, location_t l);
Queue dracula_getSafeMoves(DraculaView dv);
location_t dracula_getShortestPath(DraculaView dv, Map m, Queue dMoves, location_t target);


/* Main function*/
location_t get_dracula_move(DraculaView dv);

/* Functions used within the main function */
bool dracula_canMove(DraculaView dv, location_t move);
location_t dracula_getMoveWithinRegion(DraculaView dv, region_t r);
location_t dracula_getMoveTowardsRegion(DraculaView dv, region_t safeRegion, location_t currLocation);
bool dracula_isSafeMove(DraculaView dv, location_t l);
bool dracula_isSafeRegion(DraculaView dv, region_t currRegion);
int dracula_getSafeRegion(DraculaView dv);

void test_get_region(void);

void decide_dracula_move(DraculaView dv) {
    // if (dv_get_round(dv) == 0) { 
    //     // See if we can spawn at CD, then HI and DB --> to gain 30 health -> 70HP 
    //     // - Only do so if there aren't any hunters around - as Dracula is the last player 
        
    //     // First round 
    //     register_best_play("CF", "R U N (insert song here)"); 
    //     return; 
    // } 
    
    // if (dv_get_health(dv, PLAYER_DRACULA) <= 15) { 
    //     // Move to CD / or avoid if not safe  
    // } 
    
    // size_t nPossibleLocations; 
    // // Priority to use road than sea... 
    // location_t *possibleLocations = dv_get_dests(dv, &nPossibleLocations, true, false); 
    
    // // No road moves (nor extras) 
    // if (nPossibleLocations == 0) {
    //   possibleLocations = dv_get_dests(dv, &nPossibleLocations, true, true); 
    // } 
    
    // if (nPossibleLocations == 0) { 
    //     register_best_play("CD", "I DUN GUF AND HAVE TO GO CASTLE DRAC CYA BOIS");   
    // } else { 
    //     register_best_play(location_get_abbrev(possibleLocations[rand() % nPossibleLocations])); 
    // } 
    
    // // Okay so if nPossibleLocations is still zero we have seriously screwed up
    
    // free(possibleLocations); 
    test_get_region();
    location_t move = get_dracula_move(dv);
    register_best_play(location_get_abbrev(move), "Ya'll go away from Castle Dracula plis"); 
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
        return dracula_getMoveTowardsRegion(dv, safeRegion, currLocation);

    }
}

bool dracula_canMove(DraculaView dv, location_t move) 
{ 
    bool flag = false; 
    size_t size; 
    location_t *connections = dv_get_dests(dv, &size, true, true);
    for (size_t i = 0; i < size; i++) if (connections[i] == move) flag = true; 
    free(connections); 
    return flag; 
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

/* Checks if hunter is at any of the locations*/
bool dracula_isHunterLocation(DraculaView dv, location_t l)
{
    for (int i = 0; i < NUM_PLAYERS - 1; i++) 
        if (dv_get_location(dv, i) == l) return true; 
    return false; 
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


/* Find the shortest path to a location
* Uses a minimum spanning tree */
location_t dracula_getShortestPath(DraculaView dv, Map m, Queue dMoves, location_t target)
{
    puts("==> Getting the shortest path");
    bool hasSeen[NUM_MAP_LOCATIONS] = {false};
    location_t from[NUM_MAP_LOCATIONS] = {};
    memset(from, NUM_MAP_LOCATIONS, -1);

    Queue q = queue_new();
        // Debugging purposes
        int src = queue_de(dMoves);
        printf("==>First Item in queue: %d\n", src);
        queue_en(q, src);
    //queue_en(queue_de(dMoves));

    while (queue_size(dMoves) != 0) {
        int l = queue_de(dMoves);
        for (map_adj *n = m->connections[l]; n != NULL; n = n->next) {
            if (hasSeen[n->v] == false) {
                // Debugging
                printf("==>Checking location %d, haven't seen, got here from %d\n", n->v, l);
                hasSeen[n->v] = true;
                from[n->v] = l;
                if (n->v == target) break;
            }
            // Debugging
            printf("==>Checking location %d, seen, got here from %d\n", n->v, from[n->v]);
        }
    }
    // Debuggins
    printf("==> Finished queue, found target.. printing path: (Backwards) {");
    int tmp = (int)target;
    while (from[tmp] != -1) {
        printf("%d", tmp);
        tmp = from[tmp];
    }

    // tmp should be the value before -1
    return tmp;

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

/* Determines if a re*/
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
location_t dracula_getMoveTowardsRegion(DraculaView dv, region_t safeRegion, location_t currLocation)
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







