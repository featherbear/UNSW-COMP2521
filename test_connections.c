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
#include "map.h" 
#include "game_view.h"

#include "_dlist.h"
#include "_queue.h"

#include "__pretty.h"

// Testing Functions 
bool connections_has_location(location_t *l, location_t place);
char *connections_find_all(Map m, location_t loc);
void connections_print_array(location_t);;

// Functions for gameview (To add in later); 
size_t connections_sizeof_array(location_t *l);
location_t *connections_get_roadways (GameView gv, location_t l, enum player p, Map m);
location_t *connections_get_railways (GameView gv, location_t l, enum player p, Map m);
location_t *connections_get_seaways  (GameView gv, location_t l, enum player p, Map m);
int connections_bfs_process(Queue q, qNode n, bool *hasBeenVisited, Map m);


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
int main()
{
  
    location_t *l; 
    size_t n_locations; 
    round_t r; 
    location_t location;

    printf_blue("===== Testing for Dracula's Connections ====="); 
     
	{
  
	    // Testing rail and road.. 
	    n_locations == 0;
	    r = 0; 
	    location = SZEGET;
	    l = gv_get_connections(gv, n_locations, location, PLAYER_DRACULA, r, true, false, true);
	    {
	        printf_yellow("%s is connected to: " location_get_name(location));
	        printf("%s.\n", connections_find_all(m, SZEGET));
	        printf_yellow("Dracula can get to:")
	        connections_print_array(l);
	        
	        assert(!connections_has_location(l, HOSPITAL)); 
	        assert(!connections_has_location(l, BUCHAREST)); 
	        
	        assert(connections_has_location(l, ZAGREB));
	        assert(connections_has_location(l, BUDAPEST));
	        assert(connections_has_location(l, KLAUSENBURG));
	        assert(connections_has_location(l, BELGRADE));
	        
	        assert(n_locations == 4); 
	        free(l); 
	    }
	    
	    // Testing sea... 
	    n_locations = 0; 
	    r = 0; 
	    location = VARNA;
	    l = gv_get_connections(gv, n_locations, location, PLAYER_DRACULA, r, true false, true);

	    {
 	        printf_yellow("%s is connected to: " location_get_name(location));
	        printf("%s.\n", connections_find_all(m, SZEGET));
	        printf_yellow("Dracula can get to:")
	        connections_print_array(l);

			assert(connections_has_location(l, CONSTANTA));
	        assert(connections_has_location(l, SOFIA));
			assert(connections_has_location(l, BLACK_SEA));

	    	assert(n_locations == 3);
	    	free(l);
	    }

	    n_locations = 0;
	    r = 0;
	    location = MEDITERRANEAN_SEA;
	    l = gv_get_connections(gv, n_locations, location, PLAYER_DRACULA, r, true false, true);

	    {
 	        printf_yellow("%s is connected to: " location_get_name(location));
	        printf("%s.\n", connections_find_all(m, SZEGET));
	        printf_yellow("Dracula can get to:")
	        connections_print_array(l);

			assert(connections_has_location(l, ALICANTE));
			assert(connections_has_location(l, BARCELONA));
			assert(connections_has_location(l, MARSEILLES));
			assert(connections_has_location(l, CAGLIARI));

			asssert(n_locations == 4);
			free(l);
	    }



	}
    
    printff_blue("===== Testing for Hunter's Connections ====="); 
    
    {
    	
    }
}

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
    for (size_t i = 0; i < size; i++) printf("%s, " location_get_name(l[i])); 
}







