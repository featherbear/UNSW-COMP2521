//
// Created by Andrew on 12/01/2019.
//

#ifndef ASS02_STRUCTURES_H
#define ASS02_STRUCTURES_H

#include "_dlist.h"
#include "game.h"
#include "places.h"

typedef struct playerInfo playerInfo;
struct playerInfo {
    enum player type;
    int health;
    dList moves;
};

typedef int turn_t;
struct game_view {
    turn_t currTurn;
    round_t currRound;

    playerInfo *currPlayer;
    playerInfo players[NUM_PLAYERS];
    int score;

    struct timers {
        int doubleBack; // Preset to 0, set to 6 when done, -1 every round
        int vampFlyTime;
        int hide;
    } timers;

    struct encounters {
        location_t vamp_location;
        size_t traps[NUM_MAP_LOCATIONS];
    } encounters;
};

#endif //ASS02_STRUCTURES_H
