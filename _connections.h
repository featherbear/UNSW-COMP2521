//
// Created by Jennifer on 12/01/2019
//

#ifndef ASS02_CONNECTIONS_H
#define ASS02_CONNECTIONS_H

#include "game.h"
#include "game_view.h"
#include "map.h"
#include "places.h"
#include "_dlist.h"
#include "_queue.h"

// Credits and many thanks to
// https://stackoverflow.com/questions/10258305/how-to-implement-a-breadth-first-search-to-a-certain-depth


location_t resolveExtraLocations(dNode posNode);

//

/* Adds the extra locations (HIDE, DOUBLE_BACK, "rest" ) */
Queue connections_get_extras(GameView gv, location_t l, enum player player);

/* Checks if a location is in a player's trail */
bool location_in_trail(GameView gv, enum player p, location_t l);
bool locations_in_trail(GameView gv, enum player player, location_t *loc, size_t nLoc);

/* Finds all locations accessible by road */
Queue connections_get_roadways(GameView gv, location_t l, enum player p, Map m);

/* Finds all locations accessible by rail */
Queue connections_get_railways(location_t l, enum player p, Map m, round_t round);

/* Helper Function to find possible rail paths using BFS */
Queue connections_rail_bfs(location_t loc, Map m, int depth);

/* Helper function for BFS to enqueue items
 * Returns the number of nodes added to the queue */
int connections_bfs_process(Queue q, int item, bool *hasBeenVisited, Map m);

/* Finds all locations accessible by rail */
Queue connections_get_seaways(GameView gv, location_t l, enum player p, Map m);

bool connections_is_location_connected(Map m, location_t locA, location_t locB, bool road, bool rail, bool sea);

#endif //ASS02_CONNECTIONS_H
