////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// Places.h: an interface to the Places
//
// 2012-01-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
// // // // // // // // DO NOT CHANGE THIS FILE! // // // // // // // //

#ifndef PLACES_H_
#define PLACES_H_

// Types of places
typedef enum {
	UNKNOWN,
	LAND,
	SEA,
} PlaceType;

// Types of connections between places
typedef enum {
	NONE,
	ROAD,
	RAIL,
	BOAT,
	ANY
} TransportID;

#define MIN_TRANSPORT (TransportID)ROAD
#define MAX_TRANSPORT (TransportID)BOAT

// Places
typedef enum {
	ADRIATIC_SEA,
	ALICANTE,
	AMSTERDAM,
	ATHENS,
	ATLANTIC_OCEAN,
	BARCELONA,
	BARI,
	BAY_OF_BISCAY,
	BELGRADE,
	BERLIN,
	BLACK_SEA,
	BORDEAUX,
	BRUSSELS,
	BUCHAREST,
	BUDAPEST,
	CADIZ,
	CAGLIARI,
	CASTLE_DRACULA,
	CLERMONT_FERRAND,
	COLOGNE,
	CONSTANTA,
	DUBLIN,
	EDINBURGH,
	ENGLISH_CHANNEL,
	FLORENCE,
	FRANKFURT,
	GALATZ,
	GALWAY,
	GENEVA,
	GENOA,
	GRANADA,
	HAMBURG,
	IONIAN_SEA,
	IRISH_SEA,
	KLAUSENBURG,
	LE_HAVRE,
	LEIPZIG,
	LISBON,
	LIVERPOOL,
	LONDON,
	MADRID,
	MANCHESTER,
	MARSEILLES,
	MEDITERRANEAN_SEA,
	MILAN,
	MUNICH,
	NANTES,
	NAPLES,
	NORTH_SEA,
	NUREMBURG,
	PARIS,
	PLYMOUTH,
	PRAGUE,
	ROME,
	SALONICA,
	SANTANDER,
	SARAGOSSA,
	SARAJEVO,
	SOFIA,
	ST_JOSEPH_AND_ST_MARYS,
	STRASBOURG,
	SWANSEA,
	SZEGED,
	TOULOUSE,
	TYRRHENIAN_SEA,
	VALONA,
	VARNA,
	VENICE,
	VIENNA,
	ZAGREB,
	ZURICH,

// Other "locations"
	CITY_UNKNOWN = 100,
	SEA_UNKNOWN = 101,
	HIDE = 102,
	DOUBLE_BACK_1 = 103,
	DOUBLE_BACK_2 = 104,
	DOUBLE_BACK_3 = 105,
	DOUBLE_BACK_4 = 106,
	DOUBLE_BACK_5 = 107,
	TELEPORT = 108,
	UNKNOWN_LOCATION = -1,
	NOWHERE = UNKNOWN_LOCATION,
} LocationID;

#define MIN_MAP_LOCATION  (LocationID)ADRIATIC_SEA
#define MAX_MAP_LOCATION  (LocationID)ZURICH
#define NUM_MAP_LOCATIONS ((int)ZURICH + 1)

extern struct place {
	char *name, *abbrev;
	LocationID id;
	PlaceType type;
} PLACES[];

extern struct connection {
	LocationID v, w;
	TransportID t;
} CONNECTIONS[];

// given a Place number, return its name
char *idToName (LocationID place);

// given a Place number, return its two-char code
char *idToAbbrev (LocationID place);

// given a Place number, return its type
PlaceType idToType (LocationID place);

// given a Place name, return its ID number
LocationID nameToID (char *name);

// given a Place abbreviation, return its ID number
LocationID abbrevToID (char *abbrev);

#define isLand(place) (idToType (place) == LAND)
#define isSea(place)  (idToType (place) == SEA)
#define validPlace(pid)											\
	(MIN_MAP_LOCATION <= (pid) && (pid) <= MAX_MAP_LOCATION)

#endif // !defined (PLACES_H_)
