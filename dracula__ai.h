//
// Created by Andrew on 1/02/2019.
//

#ifndef ASS02_DRACULA_AI_H
#define ASS02_DRACULA_AI_H

#include "game.h"
#include "map.h"
#include "places.h"
#include "_queue.h"

#define NUM_REGIONS 4

typedef size_t region_t;


/** Region */
enum region {
    REGION_1,
    REGION_2,
    REGION_3,
    REGION_4
};

// Function to get the move
location_t get_dracula_move(DraculaView dv);

/* Functions called to get move .c */
bool dracula_canMove(DraculaView dv, location_t move);
location_t dracula_getMoveWithinRegion(DraculaView dv, region_t r);
location_t dracula_getMoveTowardsRegion(DraculaView dv, region_t safeRegion);
bool dracula_isSafeRegion(DraculaView dv, region_t currRegion);
int dracula_getSafeRegion(DraculaView dv);

/* Additional helper functions*/
bool *dracula_getHunterLocations(DraculaView dv);
bool *dracula_getHunterPossibleLocations(DraculaView dv);
bool *dracula_getSafeMoves(DraculaView dv);
location_t dracula_getShortestPath(DraculaView dv, Map m, Queue dMoves, location_t target);
#endif //ASS02_DRACULA_AI_H
