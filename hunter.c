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

#include "_structures.h"


#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#endif

static location_t getLastDracLocation(HunterView hv, ssize_t *distance);

// Get the last known location of Dracula
static location_t getLastDracLocation(HunterView hv, ssize_t *distance) {
    size_t nLocations;
    location_t *locations = hv_get_all_history(hv, PLAYER_DRACULA, &nLocations, true);

    location_t lastLocation = UNKNOWN_LOCATION;

    if (nLocations) {
        size_t i;
        for (i = 0; i < nLocations; i++) {
            if (valid_location_p(locations[i])) {
                *distance = (ssize_t) i;
                lastLocation = locations[i];
                break;
            }
        }
    } else {
        *distance = -1;
    }

    free(locations);
    return lastLocation;
}

void decide_hunter_move(HunterView hv) {
    enum player player = hv_get_player(hv);
    round_t round = hv_get_round(hv);

    {
        // First round, spawn hunters in regions
        if (round == 0) {
            switch (player) {
                case PLAYER_LORD_GODALMING:
                    register_best_play("PL", "");
                    return;
                case PLAYER_DR_SEWARD:
                    register_best_play("SN", "his%s%s%s%s");
                    return;
                case PLAYER_VAN_HELSING:
                    register_best_play("LI", "");
                    return;
                case PLAYER_MINA_HARKER:
                    register_best_play("SZ", "Dracula pls printf(msg) thanks");
                    return;
            }
        }
    }

    ssize_t lastDracSeen;
    location_t lastDracLocation = getLastDracLocation(hv, &lastDracSeen);

    if (lastDracSeen == -1
        && (round == 6 || round == 7)) {
        // Perform collaborative research
        register_best_play(location_get_abbrev(hv_get_location(hv, player)), "Reeeeeeee");
        return;
    }

    if (lastDracSeen > -1) {
        // Dracula was spotted - start pursuing

    }

    if (hv_get_health(hv, player) <= 4) {
        register_best_play(location_get_abbrev(hv_get_location(hv, player)), "Rest up bois");
        return;
    }

    if (hv_get_health(hv, PLAYER_DRACULA) <= 15) {
        // Dracula needs to go back to CD, unless he's really far away then stake him rather than camp
//        register_best_play("","");
    }


    size_t nPossibleLocations;
    location_t *possibleLocations = hv_get_dests(hv, &nPossibleLocations, true, true, true);
    register_best_play(location_get_abbrev(possibleLocations[(size_t)rand() % nPossibleLocations]),
                       "rand()");

    // hv_get_trail()
    // if an actual location is in the trail, start making our way to the latest location
}
