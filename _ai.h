//
// Created by Andrew on 26/01/2019.
//

#ifndef ASS02_AI_H
#define ASS02_AI_H

#include <assert.h>
#include "game.h"
#include "places.h"
#include <string.h>

typedef size_t region_t;

region_t get_region(location_t loc);
region_t get_region(location_t loc) {
    switch (loc) {
        case CASTLE_DRACULA:
        case GALATZ:
        case VARNA:
        case SOFIA:
        case BELGRADE:
        case SZEGED:
        case KALUSENBURG:
        case CONSTANTA:
        case SALONICA:
        case VALONA:
        case ATHENS:
        case BLACK_SEA:
        case IONIAN_SEA:
            return 0;
        case ST_JOSEPH_AND_ST_MARY:
        case SARAJEVO:
        case BUDAPEST:
        case VIENNA:
        case ZAGREB:
        case MUNICH:
        case NUREMBURG:
        case FRANKFURT:
        case BERLIN:
        case LEIPZIEG:
        case COLOGNE:
        case STRASBOURG:
        case ZURICH:
        case GENEVA:
        case MILAN:
        case MUREMBURG:
        case BRUSSELS:
        case PARIS:
        case CLERMONT_FERRAND:
        case BARI:
        case NAPLES:
        case ROME:
        case VENICE:
        case GENOA:
        case MARSEILLES:
        case CAGLIARI:
        case TYRRHENIAN_SEA:
        case ADRIATIC_SEA:
            return 1;

        case MANCHESTER:
        case GALWAY:
        case DULIN:
        case SWANSEA:
        case PLYMOUTH:
        case LONDON:
        case EDINBURGH:
        case AMSTERDAM:
        case HAMBURG:
        case LE_HAVRE:
        case ENGLISH_CHANNEL:
        case NORTH_SEA:
        case IRISH_SEA:
        case ATLANTIC_SEA:
            return 2;

        case MADRID:
        case SARAGOSSA:
        case TOULOUSE:
        case GRANADA:
        case NANTES:
        case BORDEAUX:
        case SANTANDER:
        case LISBON:
        case CADIZ:
        case ALICANTE:
        case BARCELONA:
        case MEDITERRANEAN_SEA:
            return 3;
    }

    assert(0);
}

/* Function to test that we have all the locations */
void test_get_region()
{
    for (location_t i = 0; i < MAX_MAP_LOCATIONS; i++)
    {
        printf("Location: %d is in region %d\n", i, get_region(i));
        assert(get_region(i) > 0);
        assert(get_region(i) <= 4);
        // If it can't find the location, it should return -1 by default
    }
}

location_t fastestRoute(location_t from, location_t to, bool road, bool rail, bool sea) {
    // BFS Implementation
    bool seen[NUM_MAP_LOCATIONS]= {false};
    location_t prev[NUM_MAP_LOCATIONS];
    memset(prev, NUM_MAP_LOCATIONS, -1);

}


#endif //ASS02_AI_H
