////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// game_view.c: GameView ADT implementation
//
// 2014-07-01   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01   v1.1    Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31   v2.0    Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "game_view.h"
#include "map.h"

#include "__pretty.h"

#include "_structures.h"
#include "_dlist.h"
#include "_queue.h"

#include "_connections.h"
#include "_game_events.h"


#define max(a, b) a>b?a:b
#define min(a, b) a<b?a:b

/* Checks whether a player (HUNTER) has rested */
static bool playerRested(GameView gv, enum player p) {

    assert(p != PLAYER_DRACULA);

    // Current location of Hunter (must exist so assert)
    dNode currNode = gv->players[p].moves->tail;
    assert(currNode);
    location_t currLocation = currNode->item;

    // Previous location of Hunter (if it exists)
    dNode prevNode = currNode->prev;
    if (!prevNode) return false;
    location_t prevLocation = currNode->prev->item;

    return currLocation == prevLocation;
}

/**
 * Creates a new view to summarise the current state of the game.
 * Plays start at index 0
 * Not doing anything with msgs for now
 */
GameView gv_new(char *past_plays, player_message messages[]) {
    if (messages) {}

    GameView gv = malloc(sizeof *gv);
    if (gv == NULL) err(EX_OSERR, "couldn't allocate GameView");

    // Initialise GameView STRUCT
    (*gv) = (struct game_view) {
            .currTurn = 0,
            .currRound = 0,
            .score = GAME_START_SCORE,

            .timers = {
                    .doubleBack = 0,
                    .vampFlyTime = 0
            },

            .encounters = {
                    .traps = {0},
                    .vamp_location = NOWHERE
            }
    };

    // Initialise PlayerInfo STRUCT
    for (size_t i = 0; i < NUM_PLAYERS; i++) {
        gv->players[i] = (playerInfo) {
                .type = (enum player) (i == PLAYER_DRACULA ? PLAYER_DRACULA : i),
                .health = (i == PLAYER_DRACULA ? GAME_START_BLOOD_POINTS : GAME_START_HUNTER_LIFE_POINTS),
                .moves = dlist_new()
        };
        dlist_push(gv->players[i].moves, UNKNOWN_LOCATION);
    }

    ////////////////////////////////////////////////////////////////
    /* Parse past_plays */
    char *tmp_pastPlays, *cursor;
    char *moveStr;

    tmp_pastPlays = cursor = strdup(past_plays);

    enum player currPlayer_n = gv->currTurn % NUM_PLAYERS;
    gv->currPlayer = &gv->players[currPlayer_n];

    while ((moveStr = strsep(&cursor, " "))) {
        if (strlen(moveStr) != 7) break;

        char player = *moveStr;           // 0
        char *_locationStr = moveStr + 1; // 1-2
        char *_event = moveStr + 1 + 2;   // 3-6


        if (currPlayer_n != PLAYER_DRACULA && gv->currPlayer->health <= 0) {
            gv->currPlayer->health = GAME_START_HUNTER_LIFE_POINTS;
        }

        // Resolve location
        char location[3] = {'\0'};
        memcpy(location, _locationStr, 2);

        location_t lID = location_find_by_abbrev(location);

        ////////////////////////////////////////////////////////////////
        /* Location */
        if (lID == -1) {

            // Possible moves for DRACULA:{UNKNOWN_CITY, UNKNOWN_SEA, HIDE, TELEPORT, DOUBLE_BACK_N}
            assert(currPlayer_n == PLAYER_DRACULA);
            if (strncmp(_locationStr, "C?", 2) == 0) {
                lID = CITY_UNKNOWN;
            } else if (strncmp(_locationStr, "S?", 2) == 0) {
                lID = SEA_UNKNOWN;
            } else if (strncmp(_locationStr, "HI", 2) == 0) {
                lID = HIDE;
            } else if (strncmp(_locationStr, "TP", 2) == 0) {
                lID = TELEPORT;
            } else if (_locationStr[0] == 'D') {
                int doubleBack_distance = _locationStr[1] - '0';
                assert(1 <= doubleBack_distance && doubleBack_distance <= 5);
                switch (doubleBack_distance) {
                    case 1:
                        lID = DOUBLE_BACK_1;
                        break;
                    case 2:
                        lID = DOUBLE_BACK_2;
                        break;
                    case 3:
                        lID = DOUBLE_BACK_3;
                        break;
                    case 4:
                        lID = DOUBLE_BACK_4;
                        break;
                    case 5:
                        lID = DOUBLE_BACK_5;
                        break;
                }
            }
        }

        dlist_push(gv->currPlayer->moves, lID);

//        printf("    Player `%c` @ `%s` (%2d) | HP: %d | %s\n", player, location, lID, gv->currPlayer->health, _event);

        ////////////////////////////////////////////////////////////////
        /* Action */
        if (player == 'D') assert(currPlayer_n == PLAYER_DRACULA);

        // Dracula ACTIONS: `T` && `V` && `M`; Trap, Vamp, Maturation
        if (currPlayer_n == PLAYER_DRACULA) {

            if (_event[0] == 'T') {
                if (valid_location_p(lID)) gv->encounters.traps[lID]++;
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
//                assert(gv->encounters.traps[lID] > 0);
                if (valid_location_p(lID)) event_remove_trap(gv, lID);

            } else if (_event[2] == 'V') {

                // Trigger vamp
                event_encounter_vamp(gv);
            }

            // Hunter ACTIONS: `T` && `V` && `D`; Trap, Vamp, Drac
        } else {

            bool isAlive = true;
            while (*_event != '.' && *_event != '\0' && isAlive) {
                switch (*_event) {
                    case 'T':
                        isAlive = event_encounter_trap(gv, currPlayer_n, lID);
                        break;
                    case 'V':
                        event_remove_vamp(gv);
                        break;
                    case 'D':
                        isAlive = event_encounter_dracula(gv, currPlayer_n);
                        break;
                }
                _event++;
            }
        }

        ////////////////////////////////////////////////////////////////
        /* Update Struct Info */

        // Dracula needs to update: Timers, Health, Score
        if (currPlayer_n == PLAYER_DRACULA) {

            // Update Timers
            if (gv->timers.vampFlyTime && --gv->timers.vampFlyTime == 0) event_encounter_vamp(gv);
            if (gv->timers.hide) --gv->timers.hide;
            if (gv->timers.doubleBack) --gv->timers.doubleBack;

            // Find the exact location
            location_t draculaLocation = resolveExtraLocations(gv->currPlayer->moves->tail);

            // Update Health: Loses 2 health everytime Drac is at sea
            if ((valid_location_p(draculaLocation) && location_get_type(draculaLocation) == SEA)
                || draculaLocation == SEA_UNKNOWN)
                event_player_hurt(gv, PLAYER_DRACULA, LIFE_LOSS_SEA);

            else if (draculaLocation == CASTLE_DRACULA) event_player_heal(gv, PLAYER_DRACULA, LIFE_GAIN_CASTLE_DRACULA);

            // Update Score: decreases by 1 each time Dracula finishes a turn.
            gv->score -= SCORE_LOSS_DRACULA_TURN;
            gv->currRound++;

            // Hunters need to update: Health
        } else {

            // Update Health: A resting player (stayed in the same city) gains 3 health
            if (playerRested(gv, currPlayer_n)) {
                event_player_heal(gv, currPlayer_n, LIFE_GAIN_REST);
            }
        }

        // Update currPlayer
        currPlayer_n = ++gv->currTurn % NUM_PLAYERS;
        gv->currPlayer = &gv->players[currPlayer_n];
    }

#if 0
    // Game summary
    printf_blue("\n\n------------SUMMARY------------\n");
    printf("    %d turns made. Now in round no %d\n    I am player: %d (%s)\n", gv->currTurn, gv->currRound, currPlayer_n,
           currPlayer_n == 4 ? "Dracula" : "Hunter");
//    printf("    Player `%c` @ `%s` | HP: %d\n", player, location, lID, _event);
    for(enum player i = 0; i < PLAYER_DRACULA; i++) {
        printf("    Hunter %d @ %-15s | HP: %d\n", i, location_get_name(gv_get_location(gv, i)), gv->players[i].health);
    }
    printf("    Dracula  @ %-15s | HP: %d\n", location_get_name(gv_get_location(gv, PLAYER_DRACULA)), gv->players[PLAYER_DRACULA].health);
    printf_yellow("    SCORE: %d\n", gv_get_score(gv));
    printf_blue("------------SUMMARY------------\n\n\n");
#endif

    ////////////////////////////////////////////////////////////////
    /* Memory Management */
    free(tmp_pastPlays);
    return gv;
}

void gv_drop(GameView gv) {
    for (size_t i = 0; i < NUM_PLAYERS; i++) {
        dlist_destroy(gv->players[i].moves);
    }
    free(gv);
}

round_t gv_get_round(GameView gv) {
    return gv->currRound;
}

enum player gv_get_player(GameView gv) {
    return gv->currTurn % NUM_PLAYERS;
}

int gv_get_score(GameView gv) {
    return gv->score;
}

int gv_get_health(GameView gv, enum player player) {
    return gv->players[player].health;
}

/* Get the current location a given player is at
 * Needs to account for whether they're at the hospital or not (Not in TRAIL) */
location_t gv_get_location(GameView gv, enum player player) {
    if (gv->players[player].health <= 0) return HOSPITAL_LOCATION;
    return gv->players[player].moves->tail->item;
}

/* Given an array, fill that array out with the given player's past moves */
void gv_get_history(GameView gv, enum player player, location_t trail[TRAIL_SIZE]) {

    dNode move = gv->players[player].moves->tail;
    for (int i = 0; i < TRAIL_SIZE; i++) {

        // Need to check if the node exists (May be less than 6 moves played)
        trail[i] = move ? move->item : -1;
        if (move) move = move->prev;
    }
}

location_t *
gv_get_connections(GameView gv, size_t *n_locations, location_t from, enum player player, round_t round, bool road,
                   bool rail, bool sea) {

    // Need to find the exact location
    if (round == gv->currRound && player == PLAYER_DRACULA) {
        from = resolveExtraLocations(gv->players[PLAYER_DRACULA].moves->tail);
    }

    assert(valid_location_p(from));
    Queue validMoves = queue_new();
    location_t *loc;
    Map m = map_new();

    // Get all the connections: {ROAD, RAIL, SEA}
    if (road) {
        Queue road_moves = connections_get_roadways(gv, from, player, m);
//         printf("\nRoad Locations  | %zu\n", queue_size(road_moves));
        queue_append_unique(validMoves, road_moves);
    }

    if (rail) {
        assert(player != PLAYER_DRACULA);
        Queue rail_moves = connections_get_railways(from, player, m, round);
//        printf("Rail Locations  | %zu\n", queue_size(rail_moves));
        queue_append_unique(validMoves, rail_moves);
    }

    if (sea) {
        Queue sea_moves = connections_get_seaways(gv, from, player, m);
//        printf("Sea Locations   | %zu\n", queue_size(sea_moves));
        queue_append_unique(validMoves, sea_moves);
    }

    // Consider extra moves
    Queue extra_moves = connections_get_extras(gv, from, player);
//    printf("Extra Locations | %zu\n", queue_size(extra_moves));
    queue_append_unique(validMoves, extra_moves);

    // If dracula doesn't have moves, he must teleport back to Castle Dracula
    size_t queueSize = queue_size(validMoves);

    if (player == PLAYER_DRACULA && queueSize == 0) {
        loc = malloc(1 * sizeof(location_t));
        loc[0] = TELEPORT;
        *n_locations = 1;

        // Put everything in the queue into the array
    } else {
        loc = malloc(queueSize * sizeof(location_t));
        for (size_t i = 0; i < queueSize; i++) loc[i] = (location_t) (size_t) queue_de(validMoves);
        *n_locations = queueSize;
    }

    // Memory Management
    queue_drop(validMoves);
    map_drop(m);

    return loc;
}
