// Created by Jennifer on 12/01/2019

#ifndef ASS02_CONNECTIONS_H
#define ASS02_CONNECTIONS_H

#include "game.h"
#include "game_view.h"
//#include "_structures.h"
#include "map.h"
#include "places.h"
//#include "_queue.h"

Queue connections_get_extras(GameView gv, location_t l, enum player player);
Queue connections_get_roadways(GameView gv, location_t l, enum player p, Map m);
Queue connections_get_railways(location_t l, enum player p, Map m, round_t round);
Queue connections_rail_bfs(location_t loc, Map m, int depth);
int connections_bfs_process(Queue q, int item, bool *hasBeenVisited, Map m);
Queue connections_get_seaways(GameView gv, location_t l, enum player p, Map m);
bool location_in_trail(GameView gv, enum player p, location_t l);

#endif //ASS02_CONNECTIONS_H
