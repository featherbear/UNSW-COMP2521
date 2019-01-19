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

// Credits and many thanks to
// https://stackoverflow.com/questions/10258305/how-to-implement-a-breadth-first-search-to-a-certain-depth

#define max(a, b) a>b?a:b
#define min(a, b) a<b?a:b

static bool playerRested(GameView gv, enum player player) {
    assert(player != PLAYER_DRACULA);

    //
    dNode currNode = gv->players[player].moves->tail;

    assert(currNode);

    location_t currLocation = currNode->item;
    dNode prevNode = currNode->prev;
    if (!prevNode) return false; // Player has not moved yet

    location_t prevLocation = currNode->prev->item;

    return currLocation == prevLocation;
}

static location_t resolveDraculaExtras(dNode draculaLocationNode) {
    location_t draculaLocation = draculaLocationNode->item;

    while (!valid_location_p(draculaLocation)) {
        if (DOUBLE_BACK_1 <= draculaLocation && draculaLocation <= DOUBLE_BACK_5) {
            for (int i = 0; i <= draculaLocationNode->item - DOUBLE_BACK_1; i++)
                draculaLocationNode = draculaLocationNode->prev;
        } else if (draculaLocation == HIDE) {
            draculaLocationNode = draculaLocationNode->prev;
        }
        draculaLocation = draculaLocationNode->item;

        switch (draculaLocation) {
            case TELEPORT:
                return CASTLE_DRACULA;
            case CITY_UNKNOWN:
            case SEA_UNKNOWN:
            case UNKNOWN_LOCATION:
                return draculaLocation;
        }
    }

    return draculaLocation;
}

GameView gv_new(char *past_plays, player_message messages[]) {
    // TODO what to do with messages

    GameView gv = malloc(sizeof *gv);
    if (gv == NULL) err(EX_OSERR, "couldn't allocate GameView");

    // Initialise GameView
    (*gv) = (struct game_view) {
            .currTurn = 0,
            .currRound = 0,
            .score = GAME_START_SCORE,

            .timers = {
                    .doubleBack = 0,
//                    .hide = 0,
                    .vampFlyTime = 0
            },

            .encounters = {
                    .traps = {0},
                    .vamp_location = NOWHERE
            }
    };

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

    enum player currPlayer_n = gv->currTurn % NUM_PLAYERS;
    gv->currPlayer = &gv->players[currPlayer_n];

    while ((moveStr = strsep(&cursor, " "))) {
        if (strlen(moveStr) != 7) break;

        char player = *moveStr;           // 0    // not too important if we're cycling n%5
        char *_locationStr = moveStr + 1; // 1-2
        char *_event = moveStr + 1 + 2;   // 3-6



        if (currPlayer_n != PLAYER_DRACULA && gv->currPlayer->health <= 0) {
            gv->currPlayer->health = GAME_START_HUNTER_LIFE_POINTS;
        }

        // Resolve location
        char location[3] = {'\0'};
        memcpy(location, _locationStr, 2);

        location_t lID = location_find_by_abbrev(location);

        /* Location */
        if (lID == -1) {
            assert(currPlayer_n == PLAYER_DRACULA);
            if (strncmp(_locationStr, "C?", 2) == 0) {
                lID = CITY_UNKNOWN;
            } else if (strncmp(_locationStr, "S?", 2) == 0) {
                lID = SEA_UNKNOWN;
            } else if (strncmp(_locationStr, "HI", 2) == 0) {
                lID = HIDE;
            } else if (strncmp(_locationStr, "TP", 2) == 0) {
                lID = CASTLE_DRACULA;
            } else if (_locationStr[0] == 'D') {
                int doubleBack_distance = _locationStr[1] - '0';
                assert(1 <= doubleBack_distance && doubleBack_distance <= 5);
                // register move: HIDE + doubleBack_distance
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


        printf("    Player `%c` @ `%s` (%2d) | HP: %d | %s\n", player, location, lID, gv->currPlayer->health, _event);

        /* Action */
        if (player == 'D') assert(currPlayer_n == PLAYER_DRACULA);

        if (currPlayer_n == PLAYER_DRACULA) {
//            assert(lID <= NUM_MAP_LOCATIONS);

            // dracula
            if (_event[0] == 'T') {
                // TODO Hunters might not know where the trap was placed
                if (valid_location_p(lID)) gv->encounters.traps[lID]++;
            }

            if (_event[1] == 'V') {
                // Place vamp
                assert(gv->encounters.vamp_location == NOWHERE);
                assert(gv->timers.vampFlyTime == 0);

                // TODO Hunters might not know where the vamp was placed

                gv->encounters.vamp_location = lID;

                gv->timers.vampFlyTime = 6; // or 7
            }

            if (_event[2] == 'M') {
                // Remove trap
                assert(gv->encounters.traps[lID] > 0);
                event_remove_trap(gv, lID);

            } else if (_event[2] == 'V') {
                // Trigger vamp
//                assert(gv->encounters.vamp_location != NOWHERE); // -> NOWHERE == UNKNOWN_LOCATION
//                assert(gv->timers.vampFlyTime > 0);

                event_encounter_vamp(gv);
            }
        } else {
            // hunter
            bool isAlive = true;
            while (*_event != '.' && *_event != '\0' && isAlive) {
                switch (*_event) {
                    case 'T':
                        // Encountered a trap
//                        assert(gv->encounters.traps[lID] > 0);
                        isAlive = event_encounter_trap(gv, currPlayer_n, lID);
                        break;
                    case 'V':
                        // Remove vamp
                        event_remove_vamp(gv);
                        break;
                    case 'D':
                        // Encounter dracula
                        isAlive = event_encounter_dracula(gv, currPlayer_n);
                        break;
                }
                _event++;
            }
        }


        if (currPlayer_n == PLAYER_DRACULA) {
            // end of round
            if (gv->timers.vampFlyTime && --gv->timers.vampFlyTime == 0) event_encounter_vamp(gv);
            if (gv->timers.hide) --gv->timers.hide;
            if (gv->timers.doubleBack) --gv->timers.doubleBack;

            // Also end of dracula's turn, take 2 damage
            dNode draculaLocationNode = gv->currPlayer->moves->tail;
            location_t draculaLocation = resolveDraculaExtras(draculaLocationNode);

            if ((valid_location_p(draculaLocation) && location_get_type(draculaLocation) == SEA)
                || draculaLocation == SEA_UNKNOWN)
                event_player_hurt(gv, PLAYER_DRACULA, LIFE_LOSS_SEA);

            else if (draculaLocation == CASTLE_DRACULA) event_player_heal(gv, PLAYER_DRACULA, LIFE_GAIN_CASTLE_DRACULA);
            // The score decreases by 1 each time Dracula finishes a turn.
            gv->score -= SCORE_LOSS_DRACULA_TURN;
            gv->currRound++;
        } else {
            // Player

            // If the current player rested (stayed in the same city), then heal
            if (playerRested(gv, currPlayer_n)) {
                event_player_heal(gv, currPlayer_n, LIFE_GAIN_REST);
            }
        }


        currPlayer_n = ++gv->currTurn % NUM_PLAYERS;
        gv->currPlayer = &gv->players[currPlayer_n];
    }

//    // Game summary
//    printf_blue("\n\n------------SUMMARY------------\n");
//    printf("    %d turns made. Now in round no %d\n    I am player: %d (%s)\n", gv->currTurn, gv->currRound, currPlayer_n,
//           currPlayer_n == 4 ? "Dracula" : "Hunter");
////    printf("    Player `%c` @ `%s` | HP: %d\n", player, location, lID, _event);
//    for(enum player i = 0; i < PLAYER_DRACULA-1; i++) {
//        printf("    Hunter %d @ %-15s | HP: %d\n", i, location_get_name(gv_get_location(gv, i)), gv->players[i].health);
//    }
//    printf("    Dracula  @ %-15s | HP: %d\n", location_get_name(gv_get_location(gv, PLAYER_DRACULA)), gv->players[PLAYER_DRACULA].health);
//    printf_yellow("    SCORE: %d\n", gv_get_score(gv));
//    printf_blue("------------SUMMARY------------\n\n\n");

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

location_t gv_get_location(GameView gv, enum player player) {
    return gv->players[player].moves->tail->item;
}

void gv_get_history(GameView gv, enum player player, location_t trail[TRAIL_SIZE]) {
    // Get the current location of the player
    dNode move = gv->players[player].moves->tail;

    // Get the most recent 6 locations into the array
    for (int i = 0; i < TRAIL_SIZE; i++) {
        // Need to check if the node exists (May be less than 6 moves played)
        trail[i] = move ? move->item : -1;
        if (move) move = move->prev;
    }
}

location_t *
gv_get_connections(GameView gv, size_t *n_locations, location_t from, enum player player, round_t round, bool road,
                   bool rail, bool sea) {

    if (round == gv->currRound && player == PLAYER_DRACULA) {
        from = resolveDraculaExtras(gv->players[PLAYER_DRACULA].moves->tail);
    }

    assert(valid_location_p(from));
    Queue validMoves = queue_new();
    location_t *loc;

    // TODO Isolate
    Map m = map_new();

    // Get all the connections
    if (road) {
        Queue road_moves = connections_get_roadways(gv, from, player, m);
        queue_append_unique(validMoves, road_moves);
    }

    if (rail) {
        assert(player != PLAYER_DRACULA);
        Queue rail_moves = connections_get_railways(from, player, m, round);
        queue_append_unique(validMoves, rail_moves);
    }

    if (sea) {
        Queue sea_moves = connections_get_seaways(gv, from, player, m);
        queue_append_unique(validMoves, sea_moves);
    }

    // Consider extra moves
    Queue extra_moves = connections_get_extras(gv, from, player);
    queue_append_unique(validMoves, extra_moves);

    // Consider the situation after piecing together information
    size_t queueSize = queue_size(validMoves);

    // If dracula doesn't have moves, it must teleport back to Castle Dracula
    if (player == PLAYER_DRACULA && queueSize == 0) {

        loc = malloc(1 * sizeof(location_t));
        loc[0] = TELEPORT;

        *n_locations = 1;

    } else {
        // Put everything in the queue into the array
        loc = malloc(queueSize * sizeof(location_t));
        for (size_t i = 0; i < queueSize; i++) loc[i] = (location_t) (size_t) queue_de(validMoves);

        *n_locations = queueSize;
    }

    queue_drop(validMoves);
    map_drop(m);

    return loc;
}




