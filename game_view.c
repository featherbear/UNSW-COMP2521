////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// game_view.c: GameView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>

#include "game.h"
#include "game_view.h"
// #include "map.h" ... if you decide to use the Map ADT

typedef struct playerInfo playerInfo;
struct playerInfo {
	enum player playerType; // playerType = HUNTER || DRACULA (?) if necessary
	int health;
	location_t currLocation; // Not saved within trail[TRAIL_SIZE]
	location_t trail[TRAIL_SIZE];
	// If the move doesn't exists, it'd return UNKNOWN_LOCATION (-1)
	// @Round 0, everyone's location is UNKNOWN_LOCATION
};

typedef struct game_view {
  round_t currRound;		// Current round number
  playerInfo currPlayer; // ---
  int score;
  playerInfo players[NUM_PLAYERS];
} game_view;

game_view *gv_new (char *past_plays, player_message messages[])
{
	game_view *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate GameView");

	(*new) = (game_view) {
		.score = GAME_START_SCORE,
		.currRound = 0, // or 1?
		
	};

	// @TODO initialise the items in the struct

	
	return new;
}

void gv_drop (game_view *gv)
{
	free (gv);
}

round_t gv_get_round (game_view *gv)
{
	return gv->currRound;
}

enum player gv_get_player (game_view *gv)
{
	return gv->currPlayer.playerType;
}

int gv_get_score (game_view *gv)
{
	return gv->score;
}

int gv_get_health (game_view *gv, enum player player)
{
	return gv->players[player].health;
}

location_t gv_get_location (game_view *gv, enum player player)
{
	return gv->players[player].currLocation; //what. who what why how go awayyyyy aaaaaaa
}

void gv_get_history (game_view *gv, enum player player, location_t trail[TRAIL_SIZE])
{
	// @TODO fill in the array --> kinda like enqueue except the trail doesn't have a head.. . we need to shift it down @_@
	// It fills in the trail for the array with the locations of the last 6 turns
	// Does that mean we fill it in as we go?
	// So at the end of every round, we need to fill in the player's history


}

location_t *gv_get_connections (game_view *gv, size_t *n_locations,	location_t from, enum player player, round_t round,
								bool road, bool rail, bool sea)
{
	*n_locations = 0;

    /*extern struct place {
        char *name, *abbrev;
        location_t id;
        place_type_t type;
    } PLACES[];
    */

/*
 *
 *    for each place
 *       if (    (P.type == ROAD  && road)
 *            || (P.type == RAIL  && rail)
 *            || (P.type == SEA   && sea)
 *          ) {
 *
 *
 *          }
 *
 */

	// Returns an array of locations that the player can potentially travel to
	// TODO create  gv_count_rounds_to func    ->> count how many rounds it takes to get from A to B, given player C

  // Need to store the amount of locations available in `n_locations`
  // For the dracula do you need to account for the trail takeback thingyabobs idk .h doesn't say so ye
  //  * It need not take into account the trail restriction.
  //TODO

	return NULL;
}
