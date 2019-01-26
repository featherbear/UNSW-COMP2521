////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" Dracula AI
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include "dracula.h"
#include "dracula_view.h"
#include "game.h"


#include <stdlib.h>

void decide_dracula_move(DraculaView dv) {

    if (dv_get_round(dv) == 0) {
        // See if we can spawn at CD, then HI and DB -> gain 30 health -> 70HP
        // - Only do so if there aren't any hunters around - as Dracula is the last player, we can check if CD is safe.

        // S

        // First round
        register_best_play("CF", "R U N (insert song here)");
        return;
    }

    if (dv_get_health(dv, PLAYER_DRACULA) < )
    /// @todo Replace this with something better!
    size_t nPossibleLocations;

    // Priority to use road than sea...
    location_t *possibleLocations = dv_get_dests(dv, &nPossibleLocations, true, false);

    // No road moves (nor extras)
    if (nPossibleLocations == 0) {
        dv_get_dests(dv, &nPossibleLocations, true, true);
    }

    if (nPossibleLocations == 0) {
        register_best_play("CD", "I DUN GUF AND HAVE TO GO CASTLE DRAC CYA BOIS");
    } else {
        register_best_play(location_get_abbrev(possibleLocations[0]),
                           "[0]");
    }
    // Okay so if nPossibleLocations is still zero we have seriously screwed up

}
