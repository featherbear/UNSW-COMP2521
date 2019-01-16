////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// hunter_view.c: the HunterView ADT implementation
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
#include "hunter_view.h"

// #include "map.h" ... if you decide to use the Map ADT

typedef struct hunter_view {
    GameView gv;
} hunter_view;

hunter_view *hv_new(char *past_plays, player_message messages[]) {
    hunter_view *
    new = malloc(sizeof *new);
    if (new == NULL) err(EX_OSERR, "couldn't allocate HunterView");

    new->gv = gv_new(past_plays, messages);

    return new;
}

void hv_drop(hunter_view *hv) {
    gv_drop(hv->gv);
    free(hv);
}

round_t hv_get_round(hunter_view *hv) {
    return gv_get_round(hv->gv);
}

enum player hv_get_player(hunter_view *hv) {
    return gv_get_player(hv->gv);
}

int hv_get_score(hunter_view *hv) {
    return gv_get_score(hv->gv);
}

int hv_get_health(hunter_view *hv, enum player player) {
    return gv_get_health(hv->gv, player);
}

location_t hv_get_location(hunter_view *hv, enum player player) {
    return gv_get_location(hv->gv, player);
}

void hv_get_trail(hunter_view *hv, enum player player, location_t trail[TRAIL_SIZE]) {
    gv_get_history(hv->gv, player, trail);
}

location_t *hv_get_dests(hunter_view *hv, size_t *n_locations, bool road, bool rail, bool sea) {
    return hv_get_dests_player(hv, n_locations, hv_get_player(hv), road, rail, sea);
}

location_t *hv_get_dests_player(hunter_view *hv, size_t *n_locations, enum player player,
                                bool road, bool rail, bool sea) {
    return gv_get_connections(hv->gv, n_locations, hv_get_location(hv, player), player, hv_get_round(hv), road, rail,
                              sea);
}
