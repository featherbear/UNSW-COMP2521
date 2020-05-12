////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula_view.c: the DraculaView ADT implementation
//
// Code by TheGroup, COMP1927 14s2.
// Modified by gac & jas, 15s2, 16s2.

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "game.h"
#include "game_view.h"
#include "dracula_view.h"

// Representation of Dracula's view of the game

struct dracula_view {
	GameView game;
	location_t myTrail[TRAIL_SIZE]; // real locations
};

// Sets dracula's trail to real places (resolving TP's, HI's and Dn's)
static void setMyTrail(char *past_plays, location_t *myTrail)
{
	int i;
	for (i = 0; i < TRAIL_SIZE; i++) myTrail[i] = NOWHERE;
	char *end;
	for (end = past_plays; *end != '\0'; end++) /*skip*/ ;
	end--;
	// we jump from Drac move to Drac move
	char *p;
	for (p = &past_plays[32]; p < end; p += 40) {
		if (*p == '\0') break;
		location_t realLoc;
		if      (p[1] == 'T' && p[2] == 'P') realLoc = CASTLE_DRACULA;
		else if (p[1] == 'H' && p[2] == 'I') realLoc = myTrail[0];
		else if (p[1] == 'D' && p[2] == '1') realLoc = myTrail[1];
		else if (p[1] == 'D' && p[2] == '2') realLoc = myTrail[2];
		else if (p[1] == 'D' && p[2] == '3') realLoc = myTrail[3];
		else if (p[1] == 'D' && p[2] == '4') realLoc = myTrail[4];
		else if (p[1] == 'D' && p[2] == '5') realLoc = myTrail[5];
		else {
			// must be a real location
			char place[3] = { p[1], p[2], '\0' };
			realLoc = location_find_by_abbrev(place);
			if (realLoc == NOWHERE) fprintf(stderr,"Ooops: %s\n",place);
		}
		// insert location at front of trail
		for (i = TRAIL_SIZE-1; i > 0; i--) myTrail[i] = myTrail[i-1];
		myTrail[0] = realLoc;
	}
#if 0
	fprintf(stderr,"myTrail:");
	for (i = 0; i < TRAIL_SIZE; i++) {
		if (myTrail[i] == NOWHERE) break;
		fprintf(stderr," %s",idToAbbrev(myTrail[i]));
	}
	fprintf(stderr,"\n");
#endif
	return;
}

// Creates a new DraculaView to summarise the current state of the game
DraculaView dv_new(char *past_plays, player_message messages[])
{
	DraculaView dracula_view = malloc(sizeof(struct dracula_view));
	dracula_view->game = gv_new(past_plays, messages);
	setMyTrail(past_plays, dracula_view->myTrail);
	return dracula_view;
}

// Frees all memory previously allocated for the DraculaView toBeDeleted
void dv_drop(DraculaView toBeDeleted)
{
	gv_drop(toBeDeleted->game);
	free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
round_t dv_get_round(DraculaView currentView)
{
	return gv_get_round(currentView->game);
}

// Get the current score
int dv_get_score(DraculaView currentView)
{
	return gv_get_score(currentView->game);
}

// Get the current health points for a given player
int dv_get_health(DraculaView currentView, enum player player)
{
	return gv_get_health(currentView->game, player);
}

// Get the current location id of a given player
location_t dv_get_location(DraculaView currentView, enum player player)
{
	if (player == PLAYER_DRACULA)
		return currentView->myTrail[0];
	else
		return gv_get_location(currentView->game, player);
}

// Get the most recent move of a given player
void dv_get_player_move(
	DraculaView currentView, enum player player,
	location_t *start, location_t *end)
{
	location_t trail[TRAIL_SIZE];
	if (player == PLAYER_DRACULA) {
		*start = currentView->myTrail[1];
		*end   = currentView->myTrail[0];
	}
	else {
		gv_get_history(currentView->game, player, trail);
		*start = trail[1];
		*end = trail[0];
	}
	return;
}

// Find out what minions are placed at the specified location
void dv_get_locale_info(
	DraculaView currentView, location_t where,
	int *numTraps, int *map_nvamps)
{
	int traps, vamps;
	getMinions(currentView->game, where, &traps, &vamps);
	*numTraps = traps;
	*map_nvamps = vamps;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void dv_get_trail(
	DraculaView currentView, enum player player,
	location_t trail[TRAIL_SIZE])
{
	if (player != PLAYER_DRACULA)
		gv_get_history(currentView->game, player, trail);
	else
		for (int i = 0; i < TRAIL_SIZE; i++)
			trail[i] = currentView->myTrail[i];
}

// Fills the trail array with the location ids of the last 6 moves
void dv_get_moves (
	DraculaView currentView, enum player player,
	location_t trail[TRAIL_SIZE])
{
	gv_get_history(currentView->game, player, trail);
}

//// Functions that query the map to find information about connectivity

// Helper function for dv_get_dests
static int onTrail(location_t *trail, location_t loc)
{
	int i;
	for (i = 1; i < TRAIL_SIZE-1; i++)
		if (trail[i] == loc) return 1;
	return 0;
}

// What are my (Dracula's) possible next moves (locations)
location_t *dv_get_dests(
	DraculaView currentView, size_t *numLocations,
	bool road, bool sea)
{
	location_t *locations =
		gv_get_connections(
			currentView->game,
			numLocations,
			currentView->myTrail[0],
			PLAYER_DRACULA,
			gv_get_round(currentView->game),
			road, false, sea);

	// find any locations that are not allowed (e.g. hospital or in trail)
	// gv_get_connections() actually removes the hospital
	// ... checking again here provides some safety and also some documentation
	size_t i, nValid = 0;
	bool *isValid = calloc(*numLocations,sizeof(int)); // assume !valid
	for (i = 0; i < *numLocations; i++) {
		if (locations[i] != ST_JOSEPH_AND_ST_MARYS
			 && !onTrail(currentView->myTrail,locations[i])) {
			isValid[i] = true;
			nValid++;
		}
	}

	// make new locations array with only valid locations
	location_t *validLocs = malloc(nValid*sizeof(location_t));
	size_t j = 0;
	for (i = 0; i < *numLocations; i++) {
		if (isValid[i]) validLocs[j++] = locations[i];
	}

	free(locations);
	*numLocations = nValid;
	return validLocs;
}

// What are the specified player's next possible moves
location_t *dv_get_dests_player(
	DraculaView currentView, size_t *numLocations,
	enum player player, bool road, bool rail, bool sea)
{
	if (player == PLAYER_DRACULA)
		return dv_get_dests(currentView, numLocations, road, sea);

	location_t *locations =
		gv_get_connections(
			currentView->game,
			numLocations,
			gv_get_location(currentView->game, player),
			player,
			gv_get_round(currentView->game),
			road, rail, sea);
	return locations;
}
