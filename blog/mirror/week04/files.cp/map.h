////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// Map.h: an interface to a Map data type
//
// 2017-11-30   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>

#ifndef MAP_H_
#define MAP_H_

#include "places.h"

typedef struct edge {
    LocationID start;
    LocationID end;
    TransportID type;
} edge;

// graph representation is hidden
typedef struct map *Map;

// operations on graphs
Map newMap (void);
void disposeMap (Map);
void showMap (Map);
int numV (Map);
int numE (Map, TransportID);

// ... for lab04:
int connections (Map g, LocationID start, LocationID end, TransportID []);

#endif // !defined(MAP_H_)
