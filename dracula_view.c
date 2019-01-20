////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula_view.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>

#include "dracula_view.h"
#include "game.h"
#include "game_view.h"

#include "_connections.h"
#include "_structures.h"


//
// Function declarations
//
static location_t get_exact_locations (location_t l);

///

typedef struct dracula_view {
    GameView gv;
} dracula_view;

///

/* Make a summary of the game situation */
dracula_view *dv_new(char *past_plays, player_message messages[]) {
    dracula_view *
    new = malloc(sizeof *new);
    if (new == NULL) err(EX_OSERR, "Couldn't allocate DraculaView");

    (*new) = (dracula_view) {
            .gv = gv_new(past_plays, messages)
    };

    return new;
}

/* Delete the struct */
void dv_drop(dracula_view *dv) {
    gv_drop(dv->gv);
    free(dv);
}

round_t dv_get_round(dracula_view *dv) {
    return gv_get_round(dv->gv);
}

int dv_get_score(dracula_view *dv) {
    return gv_get_score(dv->gv);
}

int dv_get_health(dracula_view *dv, enum player player) {
    return gv_get_health(dv->gv, player);
}

 /* Get the location of a given player */
location_t dv_get_location(dracula_view *dv, enum player player) {
    if (player == PLAYER_DRACULA) return resolveExtraLocations(dv->gv->currPlayer->moves->tail);
    return gv_get_location(dv->gv, player);
}

/* Get the most recent move of a given player */
void dv_get_player_move(dracula_view *dv, enum player player, location_t *start, location_t *end) {
    location_t *trail = malloc(sizeof(location_t) * TRAIL_SIZE);
    gv_get_history(dv->gv, player, trail);
    *start = trail[1];
    *end = trail[0];
    free(trail);
}

/* Finds out what minions are where
 * Returns the number of minions (traps and vampires)
 * `WHERE` cannot be at sea or unknown */
void dv_get_locale_info(dracula_view *dv, location_t where, int *n_traps, int *n_vamps) {
    *n_traps = (int)dv->gv->encounters.traps[where];
    *n_vamps = (int)dv->gv->encounters.vamp_location == where;
}

/* Get the history of a given player */
void dv_get_trail(dracula_view *dv, enum player player, location_t trail[TRAIL_SIZE]) {

    if (player != PLAYER_DRACULA) {
        gv_get_history(dv->gv, player, trail);
        return;
    }

    // Get the current location of the player
    dNode move = dv->gv->players[PLAYER_DRACULA].moves->tail;

    // Get the most recent 6 (or less) locations into the array
    for (int i = 0; i < TRAIL_SIZE; i++) {
        trail[i] = move ? resolveExtraLocations(move) : -1;
        if (move) move = move->prev;
    }
}

/* Get the available locations for DRACULA */
location_t *dv_get_dests(dracula_view *dv, size_t *n_locations, bool road, bool sea) {
    return gv_get_connections(dv->gv, n_locations, dv_get_location(dv, PLAYER_DRACULA), PLAYER_DRACULA, dv_get_round(dv), road, false, sea);
}

/* Get the available connections for EVERY PLAYER */
location_t *dv_get_dests_player(dracula_view *dv, size_t *n_locations, enum player player,
                                bool road, bool rail, bool sea) {
    if (player == PLAYER_DRACULA) return dv_get_dests(dv, n_locations, road, sea);

    return gv_get_connections(dv->gv, n_locations, dv_get_location(dv, player), player, dv_get_round(dv), road, rail, sea);
}
