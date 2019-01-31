////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula.h: the interface to your "Fury of Dracula" Dracula AI
//
// 2012-01-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
// // // // // // // // DO NOT CHANGE THIS FILE! // // // // // // // //

#ifndef FOD__DRACULA_H_
#define FOD__DRACULA_H_

#include "game.h"
#include "dracula_view.h"

#include "map.h"
#include "places.h"

#include "_queue.h"


//#include "_ai.h"


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
bool dracula_isSafeMove(DraculaView dv, location_t l);
bool dracula_isSafeRegion(DraculaView dv, region_t currRegion);
int dracula_getSafeRegion(DraculaView dv);

/* Additional helper functions*/
bool dracula_isHunterLocation(DraculaView dv, location_t l);
bool dracula_isHunterPossibleLocation(DraculaView dv, location_t l);
Queue dracula_getSafeMoves(DraculaView dv);
location_t dracula_getShortestPath(DraculaView dv, Map m, Queue dMoves, location_t target);

/**
 * For a given view of the game, decide what the best next move is for
 * Dracula by calling `register_best_play`.
 */
void decide_dracula_move (DraculaView state);

#endif // !defined(FOD__DRACULA_H_)
