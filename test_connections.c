//
// Written by Jennifer 12/01/19
//

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "game_view.h"
//#include "map.c"
#include "map.h"
#include "game_view.h"
#include "places.h"
#include "_queue.h"

#define BIG_SIZE 20

void get_connections_str(location_t *l, size_t size);
char *get_playerName(enum player p);


///////

int main() {

    puts("===== Testing Connections w/ Samples from Game #0 =====");

    ////////////////////////////////////////////////////////////////
    char *plays;
    player_message messages[BIG_SIZE] = {};
    GameView gv;
    enum player p;
    size_t n_loc;
    location_t from;
    location_t *l;

    puts("Game 0, Test 1");
    plays = "GMN.... SPL.... HAM.... MPA.... DC?.V..";
    {
        gv = gv_new(plays, messages);
        n_loc = 0;
        p = gv_get_player(gv);
        from = location_find_by_abbrev("MN");
//        l = gv_get_connections(gv, &n_loc, from, p, gv_get_round(gv), true, true, true);
//        printf("Round 1: PLAYER_GODALMING (0) @ MANCHESTER; 4 locations: {MANCHESTER, EDINBURGH, LIVERPOOL, LONDON}");
//        get_connections_str(l, n_loc);
        gv_drop(gv);
    }
}

void get_connections_str(location_t *l, size_t size) {
    printf("{");
    for (size_t i = 0; i < size; i++) printf("%s, ", location_get_name(l[i]));
    printf("}\n");
}

char *get_playerName(enum player p) {
    assert(p >= 0);
    assert(p < NUM_PLAYERS);
    switch (p) {
        case PLAYER_LORD_GODALMING:
            return "Lord Godalming (0)";
        case PLAYER_DR_SEWARD:
            return "Dr Seward (1)";
        case PLAYER_VAN_HELSING:
            return "Van Helsing (2)";
        case PLAYER_MINA_HARKER:
            return "Mina Harker (3)";
        case PLAYER_DRACULA:
            return "Dracula (4)";
    };
}







