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
    // Ermm... I didn't mean that kinda comment.. but yay for laziness 
    int *health = &gv->players[player].health;

    *health -= damage;
    printf("> Player %d took %d damage (HP: %d)!\n", player, damage, *health);

    if (player != PLAYER_DRACULA) {
        if (*health <= 0) {
            result = false;
            gv->score -= 6;
            printf("> Player %d dun guf. he ded. rip m9\n", player);

            // TODO ?? while in the hospital, they have zero life points
            // TODO ?? So don't reset just yet?
            *health = GAME_START_HUNTER_LIFE_POINTS;
            // TODO Move to hospital -------
            // what happens to the trail just adds hospital to trail I'm preee sure..

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

    printf("> Player %d encountered a trap!\n", player);

    event_player_hurt(gv, player, 2);
    event_remove_trap(gv, location);
}

/*
 * Vampire Encounter Event
 * Reduce the game score by 13, then remove vampire
 */
void event_encounter_vamp(game_view *gv) {
    printf("> A vampire has matured. players dun guf again smh fake hunterzz!\n");

    gv->score -= 13;
    event_remove_vamp(gv);
}

/*
 * Dracula Encounter Event
 * Damage the player by 4, and Dracula by 10
 */
void event_encounter_dracula(game_view *gv, enum player player) {
    assert(player != PLAYER_DRACULA);

    printf("> BAM! Player %d encountered Dracula at %s!\n", player, location_get_name(gv_get_location(gv, player)));

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
   /* for (size_t i = 0; i < 4; i++) {
        gv->players[i] = (playerInfo) {
                .type = (enum player) i,
                .health = GAME_START_HUNTER_LIFE_POINTS,
                .moves = dlist_new()
        };
    }
    gv->players[PLAYER_DRACULA] = (playerInfo) {
            .type = PLAYER_DRACULA,
            .health = GAME_START_BLOOD_POINTS,
            .moves = dlist_new()
    };*/

    for (size_t i = 0; i < NUM_PLAYERS; i++) {
         gv->players[i] = (playerInfo) {
                 .type = (enum player) (i == PLAYER_DRACULA ? PLAYER_DRACULA : i),
                 .health = (i == PLAYER_DRACULA ? GAME_START_BLOOD_POINTS : GAME_START_HUNTER_LIFE_POINTS),
                 .moves = dlist_new()
         };

        dlist_push(gv->players[i].moves, UNKNOWN_LOCATION);

    }


    /*
     * Parse past_plays
     */
    char *tmp_pastPlays, *cursor;
    char *moveStr;

    tmp_pastPlays = cursor = strdup(past_plays);

    while ((moveStr = strsep(&cursor, " "))) {
        if (strlen(moveStr) != 7) break;

        char player = *moveStr;           // 0
        char *_locationStr = moveStr + 1; // 1-2
        char *_event = moveStr + 1 + 2;   // 3-6

        gv->currPlayer = &gv->players[gv->currTurn % NUM_PLAYERS];

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

        printf("    Player `%c` @ `%s` (ID: %2d) | %s\n", player, location, lID, _event);

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


        if (gv->currTurn % NUM_PLAYERS == PLAYER_DRACULA) {
            // end of round
            if (gv->timers.vampFlyTime && --gv->timers.vampFlyTime == 0) event_encounter_vamp(gv);
            if (gv->timers.hide) --gv->timers.hide;
            if (gv->timers.doubleBack) --gv->timers.doubleBack;

            // Also end of dracula's turn, take 2 damage
            location_t draculaLocation = gv->currPlayer->moves->tail->item;
            if ((valid_location_p(draculaLocation) && location_get_type(gv->currPlayer->moves->tail->item) == SEA)
                || draculaLocation == SEA_UNKNOWN) event_player_hurt(gv, PLAYER_DRACULA, 2);

            // The score decreases by 1 each time Dracula finishes a turn.
            gv->score--;
            (gv->currRound)++;
        }

        gv->currTurn++;
    }

    // Game summary
    puts("\n\n------------SUMMARY------------");
    printf("    %d turns made. Now in round no %d\n    I am player: %d (%s)\n", gv->currTurn, gv->currRound, gv->currTurn % 5,
           gv->currTurn % 5 == 4 ? "Dracula" : "Hunter");
//    printf("    Player `%c` @ `%s` | HP: %d\n", player, location, lID, _event);
    for(int i = 0; i < PLAYER_DRACULA-1; i++) {
        printf("    Hunter %d @ %-15s | HP: %d\n", i, location_get_name(gv_get_location(gv, i)), gv->players[i].health);
    }
    printf("    Dracula  @ %-15s | HP: %d\n", location_get_name(gv_get_location(gv, PLAYER_DRACULA)), gv->players[PLAYER_DRACULA].health);
    printf("    SCORE: %d\n", gv_get_score(gv));
    puts("------------SUMMARY------------\n\n");

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
    DNode tailLocation = gv->players[player].moves->tail;
    return tailLocation ? tailLocation->item : UNKNOWN_LOCATION;
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

//
    return NULL;
}
/*
Draculas:
    Cannot travel by rail
    Cannot go to the hospital
    Cannot go to a location currently in his trail // So can't back back to where he came from

    Doesn't need to account for 'instantly transporting to hospital'

    Apart from going to a `location,` dracula can do a HIDE or DOUBLE_BACK
      HIDE: Stay at the current location and not move
      DOUBLE_BACK: To travel to any of the locations in the trail


Hunters:
    Moving by rail depends on sum = roundNo + hunterNo
    switch (sum % 4)
    {
        case 0: Cannot move by rail
        case 1: Can move to the adj location;
        case 2: Can move two rails;
        case 3: Can move up to three rails;

    }

For both:
    For seas, you can moves from a boat to an ajacent sea or vice verse but not port to port..


// Find all the available moves from location
// Get rid of rail for draculas
//
location_t *validMoves = connection_getLocations(game_view *gv, location_t from, size_t *n_locations, player p, Map m);

// Check for extra moves: DOUBLE_BACK & HIDE
if (player == PLAYER_DRACULA) {

    if (gv->timers.doubleBack == 0)  n_locations = connection_array_insert(validMoves, n_locations, DOUBLE_BACK);
    if (gv->timers.hide == 0)  n_locations = connection_array_insert(validMoves, n_locations, HIDE);

// Add rest as a move for hunters
} else n_locations = connection_array_insert(validMoves, n_locations, REST);

}

// Given a value, add it to the given array, and its size, assuming that the array isn't big enough
// returns the new size of the array
size_t connection_array_insert(location_t *l, size_t size, location_t value)
{
size++;
l = realloc(l, size);
l[size] = value;
return size;
}



// Get all the valid locations, considering the places that players cannot go
location_t *connection_getLocation(game_view *gv, location_t from, size_t *n_locations, player p, Map m)
{
// `l` is an array which contains the locationID of all available places
location_t *l = malloc(sizeof(*location_t));
n_locations = 0;

// Filter through all locations
map_adj tmp = m->connections[from];  // Andrew: We don't have scope to map_adj!!!
for (map_adj tmp = m->connections[from]; tmp != NULL; tmp = tmp->next) {

    // Special cases for RAIL
    if (tmp->type == RAIL) {

        if (player != PLAYER_DRACULA) {

          int sum = (int)gv_get_round(gv) + (int)gv_get_player(gv);
          switch (sum % 4)
             {
                  case 0: continue;                           // CANNOT move by rail
                  case 1: break;                              // Can only move one station
                  case 2: Can move two rails; break;          // Can move up to two stations
                  case 3: Can move up to three rails; break;  // Can move up to three stations
             };

        // Draculas cannot travel by rail
        } else continue;



    // Special cases for dracular: Can't visit the hospital and can't go back on trail (unless you call double_back)
    } else if (tmp->location == HOSPITAL) continue;
      else if (tmp->location == gv->players[PLAYER_DRACULA]->tail.location && player == PLAYER_DRACULA) continue;

    // Add the location to the array
    n_locations = connection_array_insert(l, n_locations, tmp->v);

}
}

// function gets the locationIDs of those connection to given location by rail 
connections_get_railways()





*/





