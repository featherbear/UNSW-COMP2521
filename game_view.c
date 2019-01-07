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
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "game_view.h"
// #include "map.h" ... if you decide to use the Map ADT

#include "dlist.h"



typedef struct playerInfo playerInfo;
struct playerInfo {
  enum player type;
  int health;
  DList moves;
};

typedef int turn_t;
typedef struct game_view {
    turn_t currTurn;
    round_t currRound;		// Current round number

    enum player currPlayer;
    playerInfo players[NUM_PLAYERS];

    int score;
} game_view;

//// should probably belong in the player_view.c files
//enum player whoAmI(round_t turn) {
//    switch ((turn) % 5) {
//        case 0: return PLAYER_LORD_GODALMING;
//        case 1: return PLAYER_DR_SEWARD;
//        case 2: return PLAYER_VAN_HELSING;
//        case 3: return PLAYER_MINA_HARKER;
//        case 4: return PLAYER_DRACULA;
//    }
//}
game_view *gv_new (char *past_plays, player_message messages[])
{
	game_view *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate GameView");

	(*new) = (game_view) {
        .currTurn = 0,
        .currRound = 0,

        .currPlayer = 0,

//        .players = {},

        .score = GAME_START_SCORE
	};

	// Set new healths
	for (size_t i = 0; i < 4; i++) {
	    new->players[i] = (playerInfo) {
            .type = (enum player) i,
            .health = GAME_START_HUNTER_LIFE_POINTS,
            // .trail
            .moves = dlist_new()
	    };
	}
	new->players[PLAYER_DRACULA] = (playerInfo) {
	        .type = PLAYER_DRACULA,
	        .health = GAME_START_BLOOD_POINTS,
	        // .trail
            .moves = dlist_new()
    };

	/*
	 * Parse past_plays
	 */
	char *tmp_pastPlays, *cursor;
	char *moveStr;

	tmp_pastPlays = cursor = strdup(past_plays);

	while ((moveStr = strsep(&cursor, " "))) {
	    assert(strlen(moveStr) == 7);
	    char _player = moveStr[0];

	    char location[3] = {'\0'};
	    memcpy(location, moveStr + 1, 2);
        location_t lID = location_find_by_abbrev(location);


	    char *rest = moveStr + 1 + 2;

	    printf("Player `%c` is now in location `%s` (ID: %d) actions: %s\n", _player, location, lID, rest);


	    // byref?
	    dlist_push(new->players[new->currTurn % 5].moves, lID);

	    if (++(new->currTurn) % NUM_PLAYERS == 0) (new->currRound)++;

	}

	printf("%d turns made. In round no %d\nI am player: %d\n", new->currTurn, new->currRound, new->currTurn % 5);

	free(tmp_pastPlays);

	return new;
}

void gv_drop (game_view *gv)
{
    for (size_t i = 0 ; i <NUM_PLAYERS; i++) {
    	dlist_destroy(gv->players[i].moves);
    }
	free (gv);
}

round_t gv_get_round (game_view *gv)
{
	return gv->currRound;
}

enum player gv_get_player (game_view *gv)
{
    return gv->currPlayer;
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
	return gv->players[player].moves->tail->item;
}

void gv_get_history (game_view *gv, enum player player, location_t trail[TRAIL_SIZE])
{

  // Get the current location of the player
  DNode move = gv->players[player].moves->tail;

   // Get the most recent 6 locations into the array
   for (int i = 0; i < 6; i++) {
    trail[i] = move ? move->item : -1;
    if (move) move = move->prev;
  }

  // Question: If the player is dracula, what to do when he has jumped back to a previous location, is it just added to the list?
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
