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

#include <stdlib.h>



void decide_hunter_move(HunterView hv) {
    enum player player = hv_get_player();
    if (hv_get_health(hv, PLAYER_DRACULA)) {

    }

    if (hv_get_round(hv) == 0 || hv_get_round(hv) == 1) {
        // First round
        register_best_play("CD", "SPAWN CAMP LEGGO");
        return;
    }


    size_t nPossibleLocations;

    location_t *possibleLocations = hv_get_dests(hv, &nPossibleLocations, true, true, true);
    register_best_play(location_get_abbrev(possibleLocations[rand() % nPossibleLocations]),
                       "PSEUDO RANDOM LOCATIONS WOO");

    // hv_get_trail()
    // if an actual location is in the trail, start making our way to the latest location
}
