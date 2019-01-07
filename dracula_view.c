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
// #include "map.h" ... if you decide to use the Map ADT

typedef struct dracula_view {
    GameView gv;

    struct spawnCount {
        int n_vamps;
        int n_traps;
    } *nSpawns[NUM_MAP_LOCATIONS];

} dracula_view;

dracula_view *dv_new(char *past_plays, player_message messages[]) {
    dracula_view *
    new = malloc(sizeof *new);
    if (new == NULL) err(EX_OSERR, "couldn't allocate DraculaView");

    (*new) = (dracula_view) {
            .gv = gv_new(past_plays, messages)
    };

    return new;
}

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

location_t dv_get_location(dracula_view *dv, enum player player) {
    return gv_get_location(dv->gv, player);
}

void dv_get_player_move(
        dracula_view *dv, enum player player,
        location_t *start, location_t *end) {
    start = gv_get_location(dv->gv, player);
//  end = trail[0]; // But this is ALSO within the adt? how is trail accessed?
}

void dv_get_locale_info(
        dracula_view *dv, location_t where,
        int *n_traps, int *n_vamps) {
/**
 * Find out what minions I (Dracula) have placed at the specified
 * location -- minions are traps and immature vampires -- and returns
 * the counts in the variables referenced by `n_traps` and `n_vamps`.
 *
 * If `where` is not a place where minions can be left
 * (e.g. at sea, or NOWHERE), then set both counts to zero.
 */

}

void dv_get_trail(
        dracula_view *dv, enum player player,
        location_t trail[TRAIL_SIZE]) {
    gv_get_history(dv->gv, player, trail);
    // Is the trail an array that the user has to make ? and then shove into the function?
    // If so, should we be mallocing it in gv.c?
}

location_t *dv_get_dests(dracula_view *dv, size_t *n_locations, bool road, bool sea) {
    assert(dv);
    bool rail = false;

    /* stub */ round_t _ROUND_ = 99;
    return gv_get_connections(dv->gv, n_locations, dv_get_location(dv, PLAYER_DRACULA), PLAYER_DRACULA, _ROUND_, road,
                              rail, sea);
}

location_t *dv_get_dests_player(dracula_view *dv, size_t *n_locations, enum player player,
                                bool road, bool rail, bool sea) {
    assert(dv);
    if (player == PLAYER_DRACULA) dv_get_dests(dv, n_locations, road, sea);

    /* stub */ round_t _ROUND_ = 99;
    return gv_get_connections(dv->gv, n_locations, dv_get_location(dv, player), player, _ROUND_, road, rail, sea);
}