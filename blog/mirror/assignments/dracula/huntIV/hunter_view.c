////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// hunter_view.c: the HunterView ADT implementation
//
// Code by TheGroup, COMP1927 14s2.
// Modified by gac & jas, 15s2, 16s2.

#include <stdlib.h>
#include <assert.h>

#include "game.h"
#include "game_view.h"
#include "hunter_view.h"
#include "places.h"


// Representation of the Hunter's view of the game

struct hunter_view {
    GameView game;
};


// Creates a new HunterView to summarise the current state of the game
HunterView hv_new(char *pastPlays, player_message messages[])
{
    HunterView hunter_view = malloc(sizeof(struct hunter_view));
    hunter_view->game = gv_new(pastPlays, messages);
    return hunter_view;
}


// Frees all memory previously allocated for the HunterView toBeDeleted
void hv_drop(HunterView toBeDeleted)
{
    free(toBeDeleted->game);
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
round_t hv_get_round(HunterView currentView)
{
    return gv_get_round(currentView->game);
}

// Get the id of current player
enum player hv_get_player(HunterView currentView)
{
    return gv_get_player(currentView->game);
}

// Get the current score
int hv_get_score(HunterView currentView)
{
    return gv_get_score(currentView->game);
}

// Get the current health points for a given player
int hv_get_health(HunterView currentView, enum player player)
{
    return gv_get_health(currentView->game, player);
}

// Get the current location id of a given player
location_t hv_get_location(HunterView currentView, enum player player)
{
    return gv_get_location(currentView->game, player);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void hv_get_trail(HunterView currentView, enum player player,
                    location_t trail[TRAIL_SIZE])
{
    gv_get_history(currentView->game, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
location_t *hv_get_dests(
	HunterView currentView, size_t *numLocations,
	bool road, bool rail, bool sea)
{
    return hv_get_dests_player(
		currentView,
		numLocations,
		gv_get_player(currentView->game),
		road, rail, sea);
}

// What are the specified player's next possible moves
location_t *hv_get_dests_player(
	HunterView currentView, size_t *numLocations,
	enum player player, bool road, bool rail, bool sea)
{
    size_t i, map_nvalidLocations, index;
    location_t forbidden = NOWHERE;
    location_t *validLocations;

    location_t *locations =
		gv_get_connections(
			currentView->game,
			numLocations,
			gv_get_location(currentView->game, player),
			player,
			gv_get_round(currentView->game),
			road, rail, sea);

    if (player == PLAYER_DRACULA)
		forbidden = ST_JOSEPH_AND_ST_MARYS;
    map_nvalidLocations = 0;
    for(i = 0; i < (*numLocations); i++){
        if (locations[i] == forbidden) continue;
        map_nvalidLocations++;
    }

    index = 0;
    validLocations = malloc(sizeof(location_t) * map_nvalidLocations);
    for(i = 0; i < map_nvalidLocations; i++){
        if (locations[i] == forbidden) continue;
        validLocations[index] = locations[i];
        index++;
    }

    free(locations);
    *numLocations = map_nvalidLocations;
    return validLocations;
}
