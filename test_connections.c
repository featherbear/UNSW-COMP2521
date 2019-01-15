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
#include "map.c"
#include "game_view.h"
#include "places.h"

#include "_queue.h"

#include "__pretty.h"

#define BIG_SIZE 20


print_summary(GameView gv, location_t *l, size_t size);
char *get_connections_str(location_t *l, size_t size);
char *get_playerName(enum player p);

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

    */






///////

int main()
{
    //test_get_extras();
    //test_get_roadways();
    //test_get_railways();

    printf_blue("===== Testing Connections w/ Samples from Game #0 =====");

    ////////////////////////////////////////////////////////////////
    char *plays;
    player_message messages[BIG_SIZE];
    GameView gv;
    enum player p;
    size_t n_loc;
    location_t from;
    location_t *l;

    ////////////////////////////////////////////////////////////////
    printf_yellow("Game 0, Test 1");
    plays =  "GMN.... SPL.... HAM.... MPA.... DC?.V..";
    messages["H", "E", "L", "L", "O"];
    {
        gv = gv_new(plays, messages);
        n_loc = 0;
        p = gv_get_player(gv);
        from = location_find_by_abbrev("MN");
        gv_get_connections(gv, n_loc, from, p, gv_get_round(gv), true, true, true)
        print_summary(gv, l);

    }

}


print_summary(GameView gv, location_t *l, size_t size)
{
    // Get info about the player
    enum player p = gv_get_player(gv);
    char *player = get_playerNmae(p);

    // Get info about the place
    location_t enumLocation = gv_get_location(gv, p);
    char *location = location_get_name(enumPlace);

    // Get info about the places we can get to
    char *connections = get_connections_str(l, size);

    printf("Player %s is at %s and can get to :\n%s", player, location, connections);
}

char *get_connections_str(location_t *l, size_t size)
{
    for (size_t i = 0; i < size; i++) printf("%s\n", location_get_name(l[i]));
}

char *get_playerName(enum player p)
{
    assert(p >= 0, p < NUM_PLAYERS);
    switch (p)
    {
        case PLAYER_LORD_GODALMING: return "Lord Godalming (0)"; break;
        case PLAYER_DR_SEWARD: return "Dr Seward (1)"; break;
        case PLAYER_VAN_HELSING: return "Van Helsing (2)"; break;
        case PLAYER_MINA_HARKER: return "Mina Harker (3)"; break;
        case PLAYER_DRACULA: return "Dracula (4)"; break;
    };
}







