////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include "dracula_view.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#endif

// Function to get the move
location_t get_dracula_move(DraculaView dv);

void decide_dracula_move(DraculaView dv) {
    location_t move = get_dracula_move(dv);
    register_best_play(location_get_abbrev(move), "Ya'll need to calm calm. I'm right here!");
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








