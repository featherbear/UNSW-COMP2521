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

#include "_dlist.h"
#include "_queue.h"


#include "__pretty.h"

// Testing Functions 
bool connections_has_location(location_t *l, location_t place);
char *connections_find_all(Map m, location_t loc);
void connections_print_array(location_t);
void main_test(game_view gv, size_t round_t round, location_t place, enum player player);

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

// Function returns a string off all places connected to a given place
// TODO: Test that this function works
char *connections_find_all(Map m, location_t loc)
{
    char *comp_str = "";
    map_adj tmp = m->connections[loc];
    while (tmp != NULL)
    {
        location_t l = tmp->v;
        const char *str = location_get_name (l);
        strcat(comp_str, str);
        tmp = tmp->next;
    }
    return comp_str;
}

// Function tests whether a location is located within an array
bool connections_has_location(location_t *l, location_t place)
{
    if (l == NULL) return false;
    size_t size = connections_sizeof_array(l);
    for (size_t i = 0; i < size; i++) if (l[i] == place) return true;
    return false;

}

// Function prints out all the locations
void connections_print_array(location_t *l)
{
    size_t size = connections_sizeof_array(l);
    for (size_t i = 0; i < size; i++)
        if (l[i] < MAX_NUM_LOCATIONS) printf("%s, " location_get_name(l[i]));
        // So it can't print HIDE and DOUBLE_BACK
}

location_t *main_test(game_view gv, round_t round, location_t place, enum player player)
{
    size_t n_locations = 0;
    location_t *l = gv_get_connections(gv, n_locations, place, player, round, true, false, true);
    printf_yellow("%s is connected to: " location_get_name(location));
    printf("%s.\n", connections_find_all(m, SZEGET));
    printf_yellow("Dracula can get to:")
    connections_print_array(l);
    return l;
}


void main_test(game_view gv, round_t round, location_t place, enum player player)
{
    size_t n_locations = 0;
    l = gv_get_connections(gv, n_locations, place, player, round, true, false, true);
    {



    }
}

void alter_history(game_view gv, int doubleBackTimer, int hideTimer, location_t lastPlaceVisited)
{
    gv->timers.doubleBack = doubleBackTimer;
    gv->timers.hide = hideTimer;

    dList new = dlist_new(); 
    dlist_push(new, (int)lastPlaceVisited); 

}

///////

int main()
{
    location_t *l;
    {
    printf_blue("===== Testing for Dracula's Connections =====");

    printf_red("Test 1: Rail and Hospital");
    l = main_test(gv, 0, SZEGET, PLAYER_DRACULA);

        {
            assert(!connections_has_location(l, HOSPITAL));
            assert(!connections_has_location(l, BUCHAREST));

            assert(connections_has_location(l, ZAGREB));
            assert(connections_has_location(l, BUDAPEST));
            assert(connections_has_location(l, KLAUSENBURG));
            assert(connections_has_location(l, BELGRADE));
            assert(connections_has_location(l, HIDE));

            assert(n_locations == 5);
            free(l);

        }

    printf_red("Test 2: Rail and Road");
    l = main_test(gv, 0, VARNA,  PLAYER_DRACULA);
        {
            assert(connections_has_location(l, CONSTANTA));
            assert(connections_has_location(l, SOFIA));
            assert(connections_has_location(l, BLACK_SEA));
            assert(connections_has_location(l, HIDE));

            assert(n_locations == 4);
            free(l);
        }

    printf_red("Test 3: Sea");
    l = main_test(gv, 0, MEDITERRANEAN_SEA, PLAYER_DRACULA);
        {
            assert(connections_has_location(l, ALICANTE));
    		assert(connections_has_location(l, BARCELONA));
    		assert(connections_has_location(l, MARSEILLES));
    		assert(connections_has_location(l, CAGLIARI));
    		assert(connections_has_location(l, HIDE));

    		asssert(n_locations == 5);
    		free(l);
        }

    printf_red("Test 4: Check for HIDE")
    alter_history(1, 2, GRANADA);
    l = main_test(gv, 12, MADRID, PLAYER_DRACULA);
        {
            
        }

	{

    printf_blue("===== Testing for Hunter's Connections =====");
    
    {
    	
    }
}









