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

void print_summary(GameView gv, location_t *l, size_t size);
char *get_connections_str(location_t *l, size_t size);
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

    ////////////////////////////////////////////////////////////////
    puts("Game 0, Test 1");
    plays = "GMN.... SPL.... HAM.... MPA.... DC?.V..";
    {
        gv = gv_new(plays, messages);
        n_loc = 0;
        p = gv_get_player(gv);
        from = location_find_by_abbrev("MN");
        l = gv_get_connections(gv, &n_loc, from, p, gv_get_round(gv), true, true, true);
        print_summary(gv, l, n_loc);
    }

    // TODO: Write asserts
    gv_drop(gv);
}
/*
    ////////////////////////////////////////////////////////////////
    puts("Game 0, Test 2: Hunter");
    plays =  "GMN.... SPL.... HAM.... MPA.... DC?.V.. GLV....";
    {
        gv = gv_new(plays, messages);
        n_loc = 0;
        p = gv_get_player(gv);
        from = location_find_by_abbrev("PL");
        gv_get_connections(gv, *n_loc, from, p, gv_get_round(gv), true, true, true);
        print_summary(gv, l, n_loc);
        // TODO: Write asserts

        gv_drop(gv);

    }

    ////////////////////////////////////////////////////////////////
    puts("Game 0, Test 3: Dracula");
    plays =  "MN.... SPL.... HAM.... MPA.... DZU.V.. GLV.... SLO.... HNS.... MST....";
    {
        gv = gv_new(plays, messages);
        n_loc = 0;
        p = gv_get_player(gv);
        from = location_find_by_abbrev("PL");
        gv_get_connections(gv, *n_loc, from, p, gv_get_round(gv), true, true, true);
        print_summary(gv, l, n_loc);
        // TODO: Write asserts
                gv_drop(gv);
    }
*/



// TODO: Write a location to find where a player was last at..

void print_summary(GameView gv, location_t *l, size_t size) {
    // Get info about the player
    enum player p = gv_get_player(gv);
    char *player = get_playerName(p);

    // Get info about the place
    location_t enumLocation = gv_get_location(gv, p);
    char *location = location_get_name(enumLocation);

    // Get info about the places we can get to
    char *connections = get_connections_str(l, size);

    printf("Player %s is at %s and can get to :\n%s", player, location, connections);
}

char *get_connections_str(location_t *l, size_t size) {
    for (size_t i = 0; i < size; i++) printf("%s\n", location_get_name(l[i]));
}

char *get_playerName(enum player p) {
    assert(p >= 0);
    assert(p < NUM_PLAYERS);
    switch (p) {
        case PLAYER_LORD_GODALMING:
            return "Lord Godalming (0)";
            break;
        case PLAYER_DR_SEWARD:
            return "Dr Seward (1)";
            break;
        case PLAYER_VAN_HELSING:
            return "Van Helsing (2)";
            break;
        case PLAYER_MINA_HARKER:
            return "Mina Harker (3)";
            break;
        case PLAYER_DRACULA:
            return "Dracula (4)";
            break;
    };
}







