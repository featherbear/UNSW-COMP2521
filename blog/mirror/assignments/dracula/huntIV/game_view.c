////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// game_view.c: GameView ADT implementation
//
// Code by TheGroup, COMP1927 14s2.
// Modified by gac & jas, 15s2, 16s2.

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>

#include "game.h"
#include "game_view.h"
#include "places.h"
#include "map.h"

#ifndef __unused
#define __unused __attribute__((unused))
#endif

#define TURN_LENGTH 8

// Encounterable items in cities
#define MAX_TRAPS 3
#define NO_ITEM -1

#define validPlayer(p) ((p) >= PLAYER_LORD_GODALMING && (p) <= PLAYER_DRACULA)

static int matches (char *past_plays, char *input, int index);
static int numTurns (char *past_plays);
//static void testMatches ();
static void addLocation (GameView view, enum player player, location_t newLoc);
static int seaLocation (location_t loc);

struct city {
    // Values are either NO_ITEM = -1 for not existant
	// or the turn number the item was placed
    int trap[MAX_TRAPS];
    int vamp;
};

struct game_view {
    int score;
    int lifePts[4];
    int bloodPts;

    //these are all typedefed as ints
    location_t locHistory[NUM_PLAYERS][TRAIL_SIZE];
    round_t round;
    enum player currPlayer;

    // Trap and immature vamp tracking
    struct city cities[NUM_MAP_LOCATIONS];
};

static location_t abbrevToIdExtended(char * abbrev)
{
	char abbrx[3] = { abbrev[0], abbrev[1], '\0' };

    location_t loc = location_find_by_abbrev (abbrx);
    if (loc != NOWHERE) return loc;
    int code = location_find_by_abbrev (abbrx);
    if (code >= MIN_MAP_LOCATION && code <= MAX_MAP_LOCATION) return code;
    else if(abbrx[0] == 'C' && abbrx[1] == '?') return CITY_UNKNOWN;
    else if(abbrx[0] == 'S' && abbrx[1] == '?') return SEA_UNKNOWN;
    else if(abbrx[0] == 'H' && abbrx[1] == 'I') return HIDE;
    else if(abbrx[0] == 'D' && abbrx[1] == '1') return DOUBLE_BACK_1;
    else if(abbrx[0] == 'D' && abbrx[1] == '2') return DOUBLE_BACK_2;
    else if(abbrx[0] == 'D' && abbrx[1] == '3') return DOUBLE_BACK_3;
    else if(abbrx[0] == 'D' && abbrx[1] == '4') return DOUBLE_BACK_4;
    else if(abbrx[0] == 'D' && abbrx[1] == '5') return DOUBLE_BACK_5;
    else if(abbrx[0] == 'T' && abbrx[1] == 'P') return TELEPORT;

    assert(0 && "Impossible location code");
    return NOWHERE;
}

static int originalMove (int index, char * past_plays)
{
    location_t curr = abbrevToIdExtended(&past_plays[index]+1);
    assert(past_plays[index] == 'D');

    //Dracula's location in the given trail may be a double back or hide
    //So we need to go look at the corresponding node in the trail
    while (HIDE <= curr && curr <= DOUBLE_BACK_5) {
        //If he hid, he was in the previous city
        if (curr == HIDE) {
            index -= 40;
        } else {
            assert(curr <= DOUBLE_BACK_5 && curr >= DOUBLE_BACK_1);
            //If he doubled back, go back that many steps
            index -= (curr - DOUBLE_BACK_1 + 1) * 40;
        }
        assert(past_plays[index] == 'D');
        curr = abbrevToIdExtended(&past_plays[index+1]);
    }
    return curr;
}

// Creates a new GameView to summarise the current state of the game
GameView gv_new(char *past_plays, player_message messages[] __unused)
{
    //testMatches();

    GameView view = malloc(sizeof(struct game_view));

    int i, j;
    view->score = GAME_START_SCORE;
    for (i = 0; i < NUM_PLAYERS - 1; i++)
        view->lifePts[i] = GAME_START_HUNTER_LIFE_POINTS;

    view->bloodPts = GAME_START_BLOOD_POINTS;

    for (i = 0; i < NUM_PLAYERS; i++)
        for (j = 0; j < TRAIL_SIZE; j++)
            view->locHistory[i][j] = NOWHERE;

    for (i=0; i < NUM_MAP_LOCATIONS; i++ ) {
        for (j=0; j< MAX_TRAPS; j++ )
            view->cities[i].trap[j] = NO_ITEM;
        view->cities[i].vamp = NO_ITEM;
    }

    enum player currPlayer = PLAYER_LORD_GODALMING;
    int index;

    int totalTurns = numTurns(past_plays);
    int turn = 0;
    while (turn < totalTurns) {
        index = turn * TURN_LENGTH; //index in past_plays string

        if (matches(past_plays, "D------", index)) { //drac's turn

            currPlayer = PLAYER_DRACULA;

            //score decreases when it's drac's turn
            view->score -= SCORE_LOSS_DRACULA_TURN;

            location_t newLoc = abbrevToIdExtended(&past_plays[index]+1);
            location_t resolvedMove = originalMove(index, past_plays);

            addLocation(view, PLAYER_DRACULA, newLoc);

            if (valid_location_p(resolvedMove)) {
                // Immature vampired placed
                if (matches(past_plays, "D--V---|D---V--", index)) {
                    view->cities[resolvedMove].vamp = turn;
                }
                // Trap placed
                if (matches(past_plays, "D--T---|D---T--", index)) {
					for (i = 0; i < MAX_TRAPS; i++) {
						if (view->cities[resolvedMove].trap[i] == NO_ITEM) {
							view->cities[resolvedMove].trap[i] = turn;
							break;
						}
					}
                }
            }

            //vamp matures
            if (matches(past_plays, "D----V-", index)) {
                view->score -= SCORE_LOSS_VAMPIRE_MATURES;
            }

            //drac is at sea
            if (seaLocation(resolvedMove)) {
                view->bloodPts -= LIFE_LOSS_SEA;
            }

            //drac at castle
            if (resolvedMove == TELEPORT || resolvedMove == CASTLE_DRACULA) {
                view->bloodPts += LIFE_GAIN_CASTLE_DRACULA;
            }

        } else { //hunters turn

            //determine which hunter's turn it is
            if (matches(past_plays, "G------", index)) currPlayer = PLAYER_LORD_GODALMING;
            else if (matches(past_plays, "S------", index)) currPlayer = PLAYER_DR_SEWARD;
            else if (matches(past_plays, "H------", index)) currPlayer = PLAYER_VAN_HELSING;
            else if (matches(past_plays, "M------", index)) currPlayer = PLAYER_MINA_HARKER;
            else assert(0 && "doesn't match anyone's turn");

            //setting new location of hunter
            char abbrev[3] = { past_plays[index + 1], past_plays[index + 2], 0 };
            location_t newLoc = location_find_by_abbrev(abbrev);
            int restAttempted = false;

			//hunter recovers in hospital
			if (view->lifePts[currPlayer] == 0 &&
			       gv_get_location(view, currPlayer) == ST_JOSEPH_AND_ST_MARYS)
				view->lifePts[currPlayer] = GAME_START_HUNTER_LIFE_POINTS;
            //hunter resting
            if (newLoc == gv_get_location(view, currPlayer))
                restAttempted = true;

            addLocation(view, currPlayer, newLoc);

            //hunter encounters up to 3 traps
            int trapsEncountered = 0;
            if (matches(past_plays, "---T---", index)) {
                view->lifePts[currPlayer] -= LIFE_LOSS_TRAP_ENCOUNTER;
                trapsEncountered++;
            }
            if (matches(past_plays, "----T--", index)) {
                view->lifePts[currPlayer] -= LIFE_LOSS_TRAP_ENCOUNTER;
                trapsEncountered++;
            }
            if (matches(past_plays, "-----T-", index)) {
                view->lifePts[currPlayer] -= LIFE_LOSS_TRAP_ENCOUNTER;
                trapsEncountered++;
            }
            if (matches(past_plays, "------T", index)) {
                view->lifePts[currPlayer] -= LIFE_LOSS_TRAP_ENCOUNTER;
                trapsEncountered++;
            }
            assert( trapsEncountered <= MAX_TRAPS );
            while ( trapsEncountered > 0 ) {
                trapsEncountered--;
                view->cities[newLoc].trap[trapsEncountered] = NO_ITEM;
            }

            // hunter vanquishes immature vampire
            if (matches(past_plays, "---V---|----V--|-----V-|------V", index)) {
                view->cities[newLoc].vamp = NO_ITEM;
            }

            //hunter encounters drac
            if (matches(past_plays, "---D---|----D--|-----D-|------D", index)) {
                view->lifePts[currPlayer] -= LIFE_LOSS_DRACULA_ENCOUNTER;
                view->bloodPts -= LIFE_LOSS_HUNTER_ENCOUNTER;

            }

            //hunter gets teleported to the hospital
            if (view->lifePts[currPlayer] <= 0) {
                view->score -= SCORE_LOSS_HUNTER_HOSPITAL;
                view->lifePts[currPlayer] = 0;
                view->locHistory[currPlayer][0] = ST_JOSEPH_AND_ST_MARYS;
            } else if (restAttempted) {
                view->lifePts[currPlayer] += LIFE_GAIN_REST;
                if (view->lifePts[currPlayer] > GAME_START_HUNTER_LIFE_POINTS) {
                    view->lifePts[currPlayer] = GAME_START_HUNTER_LIFE_POINTS;
                }
            }

        }

        // Cull old items/encounters in cities
        int city;
        for (city=0; city < NUM_MAP_LOCATIONS; city++) {
            for (i=0; i< MAX_TRAPS; i++) {
                if ( view->cities[city].trap[i] < turn - ((TRAIL_SIZE-1)*NUM_PLAYERS) ) {
                    view->cities[city].trap[i] = NO_ITEM;
                }
            }
            if ( view->cities[city].vamp < turn - ((TRAIL_SIZE-1)*NUM_PLAYERS) ) {
                view->cities[city].vamp = NO_ITEM;
            }
        }

        //next player's turn
        currPlayer = (currPlayer + 1) % NUM_PLAYERS;
        turn++;
    }

    //assuming round is 0 based
    view->round = turn / NUM_PLAYERS;
    view->currPlayer = currPlayer;

    return view;
}

static int seaLocation (location_t loc)
{
    if (loc == ADRIATIC_SEA
            || loc == ATLANTIC_OCEAN
            || loc == BAY_OF_BISCAY
            || loc == BLACK_SEA
            || loc == ENGLISH_CHANNEL
            || loc == IONIAN_SEA
            || loc == IRISH_SEA
            || loc == MEDITERRANEAN_SEA
            || loc == NORTH_SEA
            || loc == TYRRHENIAN_SEA
            || loc == SEA_UNKNOWN) {

        return true;
    }
    return false;
}

static void addLocation (GameView view, enum player player, location_t newLoc)
{
    //each player has an array of location_ts with length TRAIL_SIZE
    //representing the last 6 places they've been
    //add location pushes all the values down and adds a new one on the top

    assert(validPlayer(player));
    //assert(valid_location_p(from));

    int i;
    for (i = TRAIL_SIZE-1; i > 0; i--) {
        view->locHistory[player][i] = view->locHistory[player][i-1];
    }
    view->locHistory[player][0] = newLoc;
}


static int numTurns (char *past_plays)
{
    //counts the spaces
    int count = 1;
    int i = 0;
    while (past_plays[i] != 0) {
        if (past_plays[i] == ' ') {
            count++;
        }
        i++;
    }
    return ((i == 0) ? 0 : count);
}


#if 0
static void testMatches () {
    assert (matches ("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", "G------", 0) == 1);
    assert (matches ("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", "G-------", 0) == 0);
    assert (matches ("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", "G--D---", 40) == 1);
    assert (matches ("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", "S|G|H|M", 0) == 1);
    assert (matches ("GTOTD..", "---D---|----D--|-----D-|------D", 0) == 1);
    assert (matches ("DC?T.M.", "D--T---|D---T--", 0));

    assert (matches ("cat and other animals", "dog|cat", 0) == 1);
}
#endif


//returns true if the sub string matches string at the given index
// '-' is treated as any character except ' '
// '|' is treated as the OR character
// for example matches(string, "cat|dog", i) returns true if either
//the word cat OR dog is found at index i in string
static int matches (char *string, char *sub, int index)
{
    int i = 0; //position in sub string
    int isMatching = true;

    while (string[index + i] != '\0' && sub[i] != '\0') {
        if (sub[i] == '|') {
            if (isMatching) {
                return true;
            } else {
                return matches(string, &sub[i+1], index);
            }
        } else {
            if (sub[i] != '-' || string[index + i] == ' ' || string[index + i] == '\0') {
                if (sub[i] != string[index + i]) {
                    isMatching = false;
                }
            }
            i++;
        }
    }
    if (sub[i] == '|') {
        if (isMatching) {
            return true;
        } else {
            return matches(string, &sub[i+1], index);
        }
    }
    return (isMatching);
}


// Frees all memory previously allocated for the GameView toBeDeleted
void gv_drop(GameView toBeDeleted)
{
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
round_t gv_get_round(GameView currentView)
{
	return currentView->round;
}

// Get the id of current player - ie whose turn is it?
enum player gv_get_player(GameView currentView)
{
	return currentView->currPlayer;
}

// Get the current score
int gv_get_score(GameView currentView)
{
	return currentView->score;
}

// Get the current health points for a given player
int gv_get_health(GameView currentView, enum player player)
{
    assert(validPlayer(player));
    int health;
    if (player == PLAYER_DRACULA)
        health = currentView->bloodPts;
    else
        health = currentView->lifePts[player];
    return health;
}

// Get the current location id of a given player
location_t gv_get_location(GameView currentView, enum player player)
{
    assert(validPlayer(player));
    return currentView->locHistory[player][0];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void gv_get_history(GameView currentView, enum player player, location_t trail[TRAIL_SIZE])
{
    //goes through the locHistory[] for that player and copies it to trail[]
    assert(validPlayer(player));
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
        trail[i] = currentView->locHistory[player][i];
    }
}


//// Functions that query the map to find information about connectivity

// Returns an array of location_ts for all directly connected locations
location_t *gv_get_connections(
	GameView currentView __unused, size_t *numLocations,
	location_t from, enum player player, round_t round,
	bool road, bool rail, bool sea)
{
    assert(valid_location_p(from));
    assert(validPlayer(player));

    Map europe = map_new();
    int drac = (player == PLAYER_DRACULA);
    int railLength = ((int) player + (int) round) % 4;

    if (!rail) railLength = 0;
    if (drac) railLength = 0;

    location_t *res = reachable_locations(
		europe, numLocations, from,
		drac, railLength, road, sea);
    map_drop(europe);
    return res;
}

// Find out what minions are placed at the specified location
void getMinions (
	GameView game, location_t where, int *numTraps, int *map_nvamps)
{
    assert(valid_location_p(where));
    int i;

    int traps = 0;
    for (i=0; i<MAX_TRAPS; i++) {
        if (game->cities[where].trap[i] != NO_ITEM)
            traps++;
    }

    int vamps;
    vamps = (game->cities[where].vamp != NO_ITEM) ? 1 : 0;

    *numTraps = traps;
    *map_nvamps = vamps;
}
