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
#include "map.h" // ... if you decide to use the Map ADT

#include "dlist.h"

#define max(a, b) a>b?a:b
#define min(a, b) a<b?a:b // AAAAA Scary


typedef struct playerInfo playerInfo;
struct playerInfo {
    enum player type;
    int health;
    DList moves;
};

typedef int turn_t;
typedef struct game_view {
    turn_t currTurn;
    round_t currRound;        // Current round number

    playerInfo *currPlayer;
//    enum player currPlayer;
    playerInfo players[NUM_PLAYERS];
    int score;

    struct timers {
        int doubleBack; // Preset to 0, set to 6 when done, -1 every round
        int vampFlyTime;
        int hide; //
    } timers;

    struct encounters {
        location_t vamp_location;
        size_t traps[NUM_MAP_LOCATIONS];
    } encounters;

} game_view;




void event_remove_vamp(game_view *gv) {
    gv->timers.vampFlyTime = 0;
    gv->encounters.vamp_location = NOWHERE;
}

void event_remove_trap(game_view *gv, location_t location) {
    assert(gv->encounters.traps[location] > 0);

    gv->encounters.traps[location]--;
}




/*
 * Player Hurt Event
 * Reduces player health by `damage`.
 * If the player is a hunter and their health drops to zero, send them to the hospital and decrease the game score
 */
bool event_player_hurt(game_view *gv, enum player player, size_t damage) {
    bool result = true;
    // Pointer to the player's health
    // ... a POINTER because I'm lazy to keep referencing the entire gv->players[player].health each time
    int *health = &gv->players[player].health;

    *health -= damage;

    if (player != PLAYER_DRACULA) {
        if (*health <= 0) {
            result = false;
            gv->score -= 6;

            // TODO ?? while in the hospital, they have zero life points
            // TODO ?? So don't reset just yet?
            *health = GAME_START_HUNTER_LIFE_POINTS;
            // TODO Move to hospital -------
            // what happens to the trail just adds hospital to trail I'm preee sure..
//
        }
    }

    return result;
}

/*
 * Player Heal Event
 * Heals a given player - Hunters are capped at 9 health points
 */
void event_player_heal(game_view *gv, enum player player, size_t amount) {
    int *health = &gv->players[player].health;
    *health += amount;

    // Cap health for Hunters
    if (player != PLAYER_DRACULA) *health = min(9, *health);
}


/*
 * Trap Encounter Event
 * Damage the player by 2 health points, then remove trap
 */
void event_encounter_trap(game_view *gv, enum player player, location_t location) {
    assert(player != PLAYER_DRACULA);
    assert(gv->encounters.traps[location] > 0);

    event_player_hurt(gv, player, 2);
    event_remove_trap(gv, location);
}

/*
 * Vampire Encounter Event
 * Reduce the game score by 13, then remove vampire
 */
void event_encounter_vamp(game_view *gv) {
    gv->score -= 13;
    event_remove_vamp(gv);
}

/*
 * Dracula Encounter Event
 * Damage the player by 4, and Dracula by 10
 */
void event_encounter_dracula(game_view *gv, enum player player) {
    assert(player != PLAYER_DRACULA);
    event_player_hurt(gv, player, 4);
    event_player_hurt(gv, PLAYER_DRACULA, 10);
}



game_view *gv_new(char *past_plays, player_message messages[]) {
    game_view *gv = malloc(sizeof *gv);
    if (gv == NULL) err(EX_OSERR, "couldn't allocate GameView");

    // Initialise GameView
    (*gv) = (game_view) {
            .currTurn = 0,
            .currRound = 0,
            .score = GAME_START_SCORE,

            .timers = {
                    .doubleBack = 0,
                    .hide = 0,
                    .vampFlyTime = 0
            },

            .encounters = {
                    .traps = {0},
                    .vamp_location = NOWHERE
            }
    };




    // Initialise Players
    for (size_t i = 0; i < 4; i++) {
        gv->players[i] = (playerInfo) {
                .type = (enum player) i,
                .health = GAME_START_HUNTER_LIFE_POINTS,
                // .trail
                .moves = dlist_new()
        };
    }
    gv->players[PLAYER_DRACULA] = (playerInfo) {
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

        char player = *moveStr;           // 0
        char *_locationStr = moveStr + 1; // 1-2
        char *_event = moveStr + 1 + 2;   // 3-6

        gv->currPlayer = &gv->players[gv->currTurn % 5];

        // Resolve location
        char location[3] = {'\0'};
        memcpy(location, _locationStr, 2);
        location_t lID = location_find_by_abbrev(location);

        if (lID == -1) {
            if (player != 'D') {
                puts("WHAT"); // uh what happened here...
            } else {
                if (strncmp(_locationStr, "C?", 2) == 0) {
                    dlist_push(gv->currPlayer->moves, CITY_UNKNOWN);
                } else if (strncmp(_locationStr, "S?", 2) == 0) {
                    dlist_push(gv->currPlayer->moves, SEA_UNKNOWN);
                } else if (strncmp(_locationStr, "HI", 2) == 0) {
                    // TODO HIDE
                    // dlist_push(gv->players[gv->currTurn % 5].moves, CITY_UNKNOWN);
                } else if (strncmp(_locationStr, "TP", 2) == 0) {
                    dlist_push(gv->currPlayer->moves, CASTLE_DRACULA);
                } else if (_locationStr[0] == 'D') {
                    int doubleBack_distance = _locationStr[1] - '0';
                    // TODO
                }
            }
        } else {
            dlist_push(gv->currPlayer->moves, lID);
        }

        printf("Player `%c` @ `%s` (ID: %2d) | %s\n", player, location, lID, _event);

        /*
            Game Parse
         */

        if (player == 'D') {
            assert(lID <= NUM_MAP_LOCATIONS);

            // dracula
            if (_event[0] == 'T') {
                // Place trap
                assert(gv->encounters.traps[lID] < 3);

                gv->encounters.traps[lID]++;
            }
            if (_event[1] == 'V') {
                // Place vamp
                assert(gv->encounters.vamp_location == NOWHERE);
                assert(gv->timers.vampFlyTime == 0);

                gv->encounters.vamp_location = lID;
                gv->timers.vampFlyTime = 6; // or 7
            }

            if (_event[2] == 'M') {
                // Remove trap
                assert(gv->encounters.traps[lID] > 0);

                event_remove_trap(gv, lID);
            } else if (_event[2] == 'V') {
                // Trigger vamp
                assert(gv->encounters.vamp_location != NOWHERE);
                assert(gv->timers.vampFlyTime > 0);

                event_encounter_vamp(gv);
            }
        } else {
            // hunter
            while (*_event != '.' && *_event != '\0') {
                switch (*_event) {
                    case 'T':
                        // Encountered a trap
                        assert(gv->encounters.traps[lID] > 0);

                        event_encounter_trap(gv, gv->currTurn % 5, lID);
                        break;
                    case 'V':
                        // Remove vamp
                        event_remove_vamp(gv);
                        break;
                    case 'D':
                        // Encounter dracula
                        event_encounter_dracula(gv, gv->currTurn % 5);
                        break;
                }
                _event++;
            }
        }
    }


    if (++(gv->currTurn) % NUM_PLAYERS == 0) {
        // end of round
        if (gv->timers.vampFlyTime && --gv->timers.vampFlyTime == 0) event_encounter_vamp(gv);
        if (gv->timers.hide) --gv->timers.hide;
        if (gv->timers.doubleBack) --gv->timers.doubleBack;

        // Also end of dracula's turn, take 2 damage
        if (location_get_type(gv->currPlayer->moves->tail->item) == SEA) event_player_hurt(gv, PLAYER_DRACULA, 2);




        // The score decreases by 1 each time Dracula finishes a turn.
        gv->score--;
        (gv->currRound)++;
    }


    printf("%d turns made. In round no %d\nI am player: %d (%s)\n", gv->currTurn, gv->currRound, gv->currTurn % 5,
           gv->currTurn % 5 == 4 ? "Dracula" : "Hunter");

    free(tmp_pastPlays);

    return gv;
}

void gv_drop(game_view *gv) {
    for (size_t i = 0; i < NUM_PLAYERS; i++) {
        dlist_destroy(gv->players[i].moves);
    }
    free(gv);
}

round_t gv_get_round(game_view *gv) {
    return gv->currRound;
}

enum player gv_get_player(game_view *gv) {
    return gv->currTurn % 5;
}

int gv_get_score(game_view *gv) {
    return gv->score;
}

int gv_get_health(game_view *gv, enum player player) {
    return gv->players[player].health;
}

location_t gv_get_location(game_view *gv, enum player player) {
    return gv->players[player].moves->tail->item;
}

void gv_get_history(game_view *gv, enum player player, location_t trail[TRAIL_SIZE]) {

    // Get the current location of the player
    DNode move = gv->players[player].moves->tail;

    // Get the most recent 6 locations into the array
    for (int i = 0; i < 6; i++) {

        // Need to check if the node exists (May be less than 6 moves played)
        trail[i] = move ? move->item : -1;
        if (move) move = move->prev;
    }

    // Also needs consider if ther person asking is a dracula or hunter
    // Hunter sees different information to draculas
    // Hunters know if dracula is at sea
    // If the hunter enters the city where the dracula did a HIDE or DOUBLE_BACK, they know the Dracula did it
    // If the hunter treads on the Dracula's tail:
    // All the hunters can see from the 6th trail to the current trail.. NOT 100% ABOUT TRANSLATION
    //

}

location_t *
gv_get_connections(game_view *gv, size_t *n_locations, location_t from, enum player player, round_t round,
                   bool road, bool rail, bool sea) {
    *n_locations = 0;

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

/*
    1. Account for the hunters
    2. Account for the draculas

    Draculas:
        Cannot travel by rail
        Cannot go to the hospital
        Cannot go to a location currently in his trail

        Doesn't need to account for 'instantly transporting to hospital'

        Apart from going to a `location,` dracula can do a HIDE or DOUBLE_BACK
          HIDE:
          DOUBLE_BACK: To travel to any of the locations in the trail 
                TODO: Write a function bool canDoubleBack()
    */

    // TODO: Figure out the locations player can go to
    // Function accounts for the fact that draculas cannot go by rail etc
    // For seas, you can moves from a boat to an ajacent sea or vice verse but not port to port..
    // For hunters when travelling by rail.. very tricky :(
    location_t *validMoves = connection_getLocations();


    // Check for extra moves
    if (player == PLAYER_DRACULA) {
        if (gv->timers.doubleBack == 0) // TODO add DOUBLE_BACK as a valid move and realloc
            if (gv->timers.hide == 0) // TODO add HIDE as a valid move and realloc
                ;
    } else {
        // TODO add 'rest' as a valid move and realloc
    }
}

/*

typedef struct map {
	size_t n_vertices, n_edges;

	struct map_adj {
		location_t v;  // ALICANTE, etc
		transport_t type; // ROAD, RAIL, BOAT
		struct map_adj *next; // link to next node
	} *connections[NUM_MAP_LOCATIONS]; // array of lists
} map;

*/
location_t *getLocation(game_view *gv, location_t from, Map m) {
    map_adj tmp = m->connections[from];
    // Go through them and add them to the array
}
