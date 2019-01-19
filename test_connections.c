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


    plays = "MN.... SPL.... HAM.... MPA.... DZU.V.. GLV.... SL.... HNS.... MST....";
    {
        gv = gv_new(plays, messages);
        n_loc = 0;
        p = gv_get_player(gv);
        printf("The player number is: %d\n", p);
        printf("The Round is: %d\n", gv_get_round(gv));
        from = location_find_by_abbrev("ZU");
        l = gv_get_connections(gv, &n_loc, from, p, gv_get_round(gv), true, false, true);
        puts("Round 1: PLAYER_DRACULA (4) @ ZURICH; 5 locations: {STRASBOURG, GENEVA, MILAN, MUNICH, HIDE, MECEILLES}");
        get_connections_str(l, n_loc);
        printf("Number of generated locations: %d\nNumber of actual locations: 4\n", n_loc);
        free(l);
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







