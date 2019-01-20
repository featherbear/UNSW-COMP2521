////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// test_dracula_view.c: test the DraculaView ADT
//
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-02	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dracula_view.h"
#include "_testing.h"


static inline void testFramework(char *title, char *trail, struct expectedData exp) {
    T(title);
    player_message messages[] = {};
    DraculaView dv = dv_new(trail, messages);

    TEST(dv_get_round(dv) == exp.round);
    TEST(dv_get_score(dv) == exp.score);

    {
        TEST(dv_get_health(dv, PLAYER_LORD_GODALMING) == exp.health[PLAYER_LORD_GODALMING]);
        TEST(dv_get_health(dv, PLAYER_DR_SEWARD) == exp.health[PLAYER_DR_SEWARD]);
        TEST(dv_get_health(dv, PLAYER_VAN_HELSING) == exp.health[PLAYER_VAN_HELSING]);
        TEST(dv_get_health(dv, PLAYER_MINA_HARKER) == exp.health[PLAYER_MINA_HARKER]);
        TEST(dv_get_health(dv, PLAYER_DRACULA) == exp.health[PLAYER_DRACULA]);
    }

    // TODO: CHECK dv_get_player_move(dv, player, *start, *end);

    location_t start;
    location_t end;
    {
        dv_get_player_move(dv, PLAYER_LORD_GODALMING, &start, &end);
            assert(exp.history[PLAYER_LORD_GODALMING][0] == start);
            assert(exp.history[PLAYER_LORD_GODALMING][1] == end);

        dv_get_player_move(dv, PLAYER_DR_SEWARD, &start, &end);
            assert(exp.history[PLAYER_DR_SEWARD][0] == start);
            assert(exp.history[PLAYER_DR_SEWARD][1] == end);

        dv_get_player_move(dv, PLAYER_VAN_HELSING, &start, &end);
            assert(exp.history[PLAYER_VAN_HELSING][0] == start);
            assert(exp.history[PLAYER_VAN_HELSING][1] == end);

        dv_get_player_move(dv, PLAYER_MINA_HARKER, &start, &end);
            assert(exp.history[PLAYER_MINA_HARKER][0] == start);
            assert(exp.history[PLAYER_MINA_HARKER][1] == end);

        dv_get_player_move(dv, PLAYER_DRACULA, &start, &end);
            assert(exp.history[PLAYER_DRACULA][0] == start);
            assert(exp.history[PLAYER_DRACULA][1] == end);
    }


    // TODO: dc_get_locale_info(dv, where, *n_traps, *n_vamps);

    int n_traps;
    int n_vamps;

    location_t history[TRAIL_SIZE];
    {
        A("Check Lord Godalming's move history");
        dv_get_trail(dv, PLAYER_LORD_GODALMING, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_LORD_GODALMING][i]);
        O();
    }
    {
        A("Check Dr Seward's move history");
        dv_get_trail(dv, PLAYER_DR_SEWARD, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_DR_SEWARD][i]);
        O();
    }
    {
        A("Check Van Helsing's move history");
        dv_get_trail(dv, PLAYER_VAN_HELSING, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_VAN_HELSING][i]);
        O();
    }
    {
        A("Check Mina Harker's move history");
        dv_get_trail(dv, PLAYER_MINA_HARKER, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_MINA_HARKER][i]);
        O();
    }
    {
        A("Check Dracula's move history");
        dv_get_trail(dv, PLAYER_DRACULA, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_DRACULA][i]);
        O();
    }

    // TODO: Check this.
    // location_t *dv_get_dests_player (DraculaView dv, size_t *n_locations, enum player player, bool road, bool rail, bool sea);

    if (exp.round > 0) {
        A("Check connections for current player");
        size_t nConnections;

        // Left it as true because draula one will automatically become false
        location_t *connections = dv_get_dests_player(dv, &nConnections, exp.player, true, true, true);
        TEST(arraysEqual(nConnections, connections, exp.nConnections, exp.connections));
        free(connections);
        O();
    }

    gv_drop(dv);
}
int main(void) {
    //TODO
   // testFramework(char *title, char *trail, struct expectedData exp);

    do {////////////////////////////////////////////////////////////////
        puts("Test for basic functions, "
             "just before Dracula's first move");

        char *trail =
                "GST.... SAO.... HZU.... MBB....";
        player_message messages[] = {
                "Hello", "Rubbish", "Stuff", ""};
        DraculaView dv = dv_new(trail, messages);

        assert(dv_get_round(dv) == 0);
        assert(dv_get_location(dv, PLAYER_LORD_GODALMING) == STRASBOURG);
        assert(dv_get_location(dv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
        assert(dv_get_location(dv, PLAYER_VAN_HELSING) == ZURICH);
        assert(dv_get_location(dv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
        assert(dv_get_location(dv, PLAYER_DRACULA) == UNKNOWN_LOCATION);
        assert(dv_get_health(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);

        puts("passed");
        dv_drop(dv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts("Test for encountering Dracula and hunter history");

        char *trail =
                "GST.... SAO.... HCD.... MAO.... DGE.... "
                "GGED... SAO.... HCD.... MAO....";
        player_message messages[] = {
                "Hello", "Rubbish", "Stuff", "", "Mwahahah",
                "Aha!", "", "", ""};
        DraculaView dv = dv_new(trail, messages);

        assert(dv_get_round(dv) == 1);
        assert(dv_get_location(dv, PLAYER_DRACULA) == GENEVA);
        assert(dv_get_health(dv, PLAYER_LORD_GODALMING) == 5);
        assert(dv_get_health(dv, PLAYER_DRACULA) == 30);
        assert(dv_get_location(dv, PLAYER_LORD_GODALMING) == GENEVA);

        location_t history[TRAIL_SIZE];
        dv_get_trail(dv, PLAYER_DRACULA, history);
        assert(history[0] == GENEVA);
        assert(history[2] == UNKNOWN_LOCATION);

        dv_get_trail(dv, PLAYER_LORD_GODALMING, history);
        assert(history[0] == GENEVA);
        assert(history[1] == STRASBOURG);
        assert(history[2] == UNKNOWN_LOCATION);

        dv_get_trail(dv, PLAYER_DR_SEWARD, history);
        assert(history[0] == ATLANTIC_OCEAN);
        assert(history[1] == ATLANTIC_OCEAN);
        assert(history[2] == UNKNOWN_LOCATION);

        puts("passed");
        dv_drop(dv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts("Test for Dracula leaving minions");

        char *trail =
                "GGE.... SGE.... HGE.... MGE.... DED.V.. "
                "GST.... SST.... HST.... MST.... DMNT... "
                "GST.... SST.... HST.... MST....";
        player_message messages[] = {
                "Hello", "Rubbish", "Stuff", "", "Mwahahah",
                "Aha!", "", "", "", "Drop a V",
                "Party in Strasbourg", "Party", "Party", "Party"};
        DraculaView dv = dv_new(trail, messages);

        int nT, nV;
        dv_get_locale_info(dv, EDINBURGH, &nT, &nV);
        assert(nT == 0 && nV == 1);
        dv_get_locale_info(dv, MANCHESTER, &nT, &nV);
        assert(nT == 1 && nV == 0);
        assert(dv_get_location(dv, PLAYER_DRACULA) == MANCHESTER);

        location_t history[TRAIL_SIZE];
        dv_get_trail(dv, PLAYER_DRACULA, history);
        assert(history[0] == MANCHESTER);
        assert(history[1] == EDINBURGH);
        assert(history[2] == UNKNOWN_LOCATION);

        dv_get_trail(dv, PLAYER_MINA_HARKER, history);
        assert(history[0] == STRASBOURG);
        assert(history[1] == STRASBOURG);
        assert(history[2] == GENEVA);
        assert(history[3] == UNKNOWN_LOCATION);

        puts("passed");
        dv_drop(dv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts("Checking Galatz road connections");
        char *trail = "GGA....";
        player_message messages[] = {"Gone to Galatz"};
        DraculaView dv = dv_new(trail, messages);

        size_t n_edges;
        location_t *edges = dv_get_dests_player(
                dv, &n_edges, PLAYER_LORD_GODALMING, true, false, false);
        bool seen[NUM_MAP_LOCATIONS] = {false};
        for (size_t i = 0; i < n_edges; i++)
            seen[edges[i]] = true;

        assert(n_edges == 5);
        assert(seen[GALATZ]);
        assert(seen[CONSTANTA]);
        assert(seen[BUCHAREST]);
        assert(seen[KLAUSENBURG]);
        assert(seen[CASTLE_DRACULA]);

        puts("passed");
        free(edges);
        dv_drop(dv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts("Checking Ionian Sea sea connections");
        char *trail = "GIO....";
        player_message messages[] = {"Sailing the Ionian"};
        DraculaView dv = dv_new(trail, messages);

        size_t n_edges;
        location_t *edges = dv_get_dests_player(
                dv, &n_edges, PLAYER_LORD_GODALMING, false, false, true);
        bool seen[NUM_MAP_LOCATIONS] = {false};
        for (size_t i = 0; i < n_edges; i++)
            seen[edges[i]] = true;

        assert(n_edges == 7);
        assert(seen[IONIAN_SEA]);
        assert(seen[BLACK_SEA]);
        assert(seen[ADRIATIC_SEA]);
        assert(seen[TYRRHENIAN_SEA]);
        assert(seen[ATHENS]);
        assert(seen[VALONA]);
        assert(seen[SALONICA]);

        puts("passed");
        free(edges);
        dv_drop(dv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts("Checking Athens rail connections (none)");

        char *trail = "GAT....";
        player_message messages[] = {"Leaving Athens by train"};
        DraculaView dv = dv_new(trail, messages);

        size_t n_edges;
        location_t *edges = dv_get_dests_player(
                dv, &n_edges, PLAYER_LORD_GODALMING, false, true, false);

        assert(n_edges == 1);
        assert(edges[0] == ATHENS);

        puts("passed");
        free(edges);
        dv_drop(dv);
    } while (0);

    //////////////////////////////////////////////////////////////
    /* Rolling Tests */

    {
        testFramework("Round 0 Turn 0", "", (struct expectedData) {
                .player = PLAYER_LORD_GODALMING,
                .score = 366,
                .round = 0,

                .location = {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                .history = {{UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = NULL,
                .nConnections = 0,
        });

    }
    {
        testFramework("Round 0 Turn 1", "GMN....", (struct expectedData) {
                .player = PLAYER_DR_SEWARD,
                .score = 366,
                .round = 0,

                .location = {MANCHESTER, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                .history = {{MANCHESTER,       UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = NULL,
                .nConnections = 0,
        });
    }
    {
        testFramework("Round 0 Turn 2", "GMN.... SPL....", (struct expectedData) {
                .player = PLAYER_VAN_HELSING,
                .score = 366,
                .round = 0,

                .location = {MANCHESTER, PLYMOUTH, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                .history = {{MANCHESTER,       UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PLYMOUTH,         UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = NULL,
                .nConnections = 0
        });
    }
    {
        testFramework("Round 0 Turn 3", "GMN.... SPL.... HAM....", (struct expectedData) {
                .player = PLAYER_MINA_HARKER,
                .score = 366,
                .round = 0,

                .location = {MANCHESTER, PLYMOUTH, AMSTERDAM, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                .history = {{MANCHESTER,       UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PLYMOUTH,         UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {AMSTERDAM,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = NULL,
                .nConnections = 0,
        });
    }
    {
        testFramework("Round 0 Turn 4", "GMN.... SPL.... HAM.... MPA....", (struct expectedData) {
                .player = PLAYER_DRACULA,
                .score = 366,
                .round = 0,

                .location = {MANCHESTER, PLYMOUTH, AMSTERDAM, PARIS, UNKNOWN_LOCATION},
                .history = {{MANCHESTER,       UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PLYMOUTH,         UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {AMSTERDAM,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PARIS,            UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = NULL,
                .nConnections = 0,
        });
    }
    {
        // Next round
        location_t connections[] = {MANCHESTER, EDINBURGH, LIVERPOOL, LONDON};
        testFramework("Round 1 Turn 0", "GMN.... SPL.... HAM.... MPA.... DC?.V..", (struct expectedData) {
                // TODO VAMPIRE WAS PLACED / Check drac view
                .player = PLAYER_LORD_GODALMING,
                .score = 365,
                .round = 1,

                .location = {MANCHESTER, PLYMOUTH, AMSTERDAM, PARIS, CITY_UNKNOWN},
                .history = {{MANCHESTER,   UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PLYMOUTH,     UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {AMSTERDAM,    UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PARIS,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {CITY_UNKNOWN, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = connections,
                .nConnections = 4,
        });
    }
    {
        location_t connections[] = {PLYMOUTH, LONDON, ENGLISH_CHANNEL};
        testFramework("Round 1 Turn 1", "GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                                        "GLV....", (struct expectedData) {
                .player = PLAYER_DR_SEWARD,
                .score = 365,
                .round = 1,

                .location = {LIVERPOOL, PLYMOUTH, AMSTERDAM, PARIS, CITY_UNKNOWN},
                .history = {{LIVERPOOL,    MANCHESTER,       UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PLYMOUTH,     UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {AMSTERDAM,    UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PARIS,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {CITY_UNKNOWN, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = connections,
                .nConnections = 3,
        });
    }
    {
        location_t connections[] = {AMSTERDAM, NORTH_SEA, BRUSSELS, COLOGNE};
        testFramework("Round 1 Turn 2", "GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                                        "GLV.... SLO....", (struct expectedData) {
                .player = PLAYER_VAN_HELSING,
                .score = 365,
                .round = 1,

                .location = {LIVERPOOL, LONDON, AMSTERDAM, PARIS, CITY_UNKNOWN},
                .history = {{LIVERPOOL,    MANCHESTER,       UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {LONDON,       PLYMOUTH,         UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {AMSTERDAM,    UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PARIS,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {CITY_UNKNOWN, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = connections,
                .nConnections = 4,
        });
    }
    {
        location_t connections[] = {PARIS, GENEVA, CLERMONT_FERRAND, NANTES, LE_HAVRE, BRUSSELS, STRASBOURG};
        testFramework("Round 1 Turn 3", "GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                                        "GLV.... SLO.... HNS....", (struct expectedData) {
                .player = PLAYER_MINA_HARKER,
                .score = 365,
                .round = 1,

                .location = {LIVERPOOL, LONDON, NORTH_SEA, PARIS, CITY_UNKNOWN},
                .history = {{LIVERPOOL,    MANCHESTER,       UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {LONDON,       PLYMOUTH,         UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {NORTH_SEA,    AMSTERDAM,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {PARIS,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {CITY_UNKNOWN, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},

                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = connections,
                .nConnections = 7,
        });
    }
    {
        location_t connections[] = {STRASBOURG, GENEVA, MILAN, MUNICH, MARSEILLES, HIDE};
        testFramework("Round 1 Turn 4", "GMN.... SPL.... HAM.... MPA.... DZU.V.. "
                                        "GLV.... SLO.... HNS.... MST....", (struct expectedData) {
                .player = PLAYER_DRACULA,
                .score = 365,
                .round = 1,

                .location = {LIVERPOOL, LONDON, NORTH_SEA, STRASBOURG, ZURICH},
                .history = {{LIVERPOOL,  MANCHESTER,       UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {LONDON,     PLYMOUTH,         UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {NORTH_SEA,  AMSTERDAM,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {STRASBOURG, PARIS,            UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {ZURICH,     UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = connections,
                .nConnections = 6,
        });
    }

    // Next round

    {
        location_t connections[] = {LIVERPOOL, SWANSEA, MANCHESTER, IRISH_SEA, EDINBURGH, LONDON};
        testFramework("Round 2 Turn 0", "GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                                        "GLV.... SLO.... HNS.... MST.... DC?T...", (struct expectedData) {
                .player = PLAYER_LORD_GODALMING,
                .score = 364,
                .round = 2,

                .location = {LIVERPOOL, LONDON, NORTH_SEA, STRASBOURG, CITY_UNKNOWN},
                .history = {{LIVERPOOL,    MANCHESTER,   UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {LONDON,       PLYMOUTH,     UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {NORTH_SEA,    AMSTERDAM,    UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {STRASBOURG,   PARIS,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {CITY_UNKNOWN, CITY_UNKNOWN, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = connections,
                .nConnections = 6,
        });
    }
    {
        location_t connections[] = {LONDON, MANCHESTER, EDINBURGH, LIVERPOOL, SWANSEA, ENGLISH_CHANNEL, PLYMOUTH};
        testFramework("Round 2 Turn 1", "GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                                        "GLV.... SLO.... HNS.... MST.... DC?T... "
                                        "GIR....", (struct expectedData) {
                .player = PLAYER_DR_SEWARD,
                .score = 364,
                .round = 2,

                .location = {IRISH_SEA, LONDON, NORTH_SEA, STRASBOURG, CITY_UNKNOWN},
                .history = {{IRISH_SEA,    LIVERPOOL,    MANCHESTER,       UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {LONDON,       PLYMOUTH,     UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {NORTH_SEA,    AMSTERDAM,    UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {STRASBOURG,   PARIS,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {CITY_UNKNOWN, CITY_UNKNOWN, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS},

                .connections = connections,
                .nConnections = 7,
        });
    }

    /* */

    {
        location_t connections[] = {STRASBOURG, MUNICH, MILAN, MARSEILLES, GENEVA, ZURICH};
        testFramework("Round 5 Turn 3", "GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                                        "GLV.... SLO.... HNS.... MST.... DC?T... "
                                        "GIR.... SPL.... HAO.... MZU.... DCDT... "
                                        "GSW.... SLO.... HNS.... MFR.... DC?T... "
                                        "GLV.... SPL.... HAO.... MZU.... DC?T... "
                                        "GSW.... SLO.... HNS....", (struct expectedData) {
                .player = PLAYER_MINA_HARKER,
                .score = 361,
                .round = 5,

                .location = {SWANSEA, LONDON, NORTH_SEA, ZURICH, CITY_UNKNOWN},
                .history = {{SWANSEA,      LIVERPOOL,      SWANSEA,        IRISH_SEA,      LIVERPOOL,    MANCHESTER},
                            {LONDON,       PLYMOUTH,       LONDON,         PLYMOUTH,       LONDON,       PLYMOUTH},
                            {NORTH_SEA,    ATLANTIC_OCEAN, NORTH_SEA,      ATLANTIC_OCEAN, NORTH_SEA,    AMSTERDAM},
                            {ZURICH,       FRANKFURT,      ZURICH,         STRASBOURG,     PARIS,        UNKNOWN_LOCATION},
                            {CITY_UNKNOWN, CITY_UNKNOWN,   CASTLE_DRACULA, CITY_UNKNOWN,   CITY_UNKNOWN, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS + LIFE_GAIN_CASTLE_DRACULA},

                .connections = connections,
                .nConnections = 6,
        });
    }
    {
        location_t connections[] = {CONSTANTA, BELGRADE, SOFIA, DOUBLE_BACK_1, DOUBLE_BACK_2, DOUBLE_BACK_3,
                                    DOUBLE_BACK_4};
        testFramework("Round 5 Turn 4", "GMN.... SPL.... HAM.... MPA.... DGA.V.. "
                                        "GLV.... SLO.... HNS.... MST.... DHIT... "
                                        "GIR.... SPL.... HAO.... MZU.... DCDT... "
                                        "GSW.... SLO.... HNS.... MFR.... DKLT... "
                                        "GLV.... SPL.... HAO.... MZU.... DBCT... "
                                        "GSW.... SLO.... HNS.... MMR....", (struct expectedData) {
                .player = PLAYER_DRACULA,
                .score = 361,
                .round = 5,

                .location = {SWANSEA, LONDON, NORTH_SEA, MARSEILLES, BUCHAREST},
                .history = {{SWANSEA,    LIVERPOOL,      SWANSEA,        IRISH_SEA,      LIVERPOOL,  MANCHESTER},
                            {LONDON,     PLYMOUTH,       LONDON,         PLYMOUTH,       LONDON,     PLYMOUTH},
                            {NORTH_SEA,  ATLANTIC_OCEAN, NORTH_SEA,      ATLANTIC_OCEAN, NORTH_SEA,  AMSTERDAM},
                            {MARSEILLES, ZURICH,         FRANKFURT,      ZURICH,         STRASBOURG, PARIS},
                            {BUCHAREST,  KLAUSENBURG,    CASTLE_DRACULA, HIDE,           GALATZ,     UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS + LIFE_GAIN_CASTLE_DRACULA},

                .connections = connections,
                .nConnections = 7,
        });
    }

    /* */

    {
        location_t connections[] = {PLYMOUTH, ENGLISH_CHANNEL, LONDON};
        testFramework("Round 7 Turn 0", "GED.... SGE.... HZU.... MCA.... DCF.V.. "
                                        "GMN.... SCFVD.. HGE.... MLS.... DBOT... " // S - 4; D - 10
                                        "GLO.... SMR.... HCF.... MMA.... DC?T... "
                                        "GPL.... SMS.... HMR.... MGR.... DBAT... " // S - 4 - 2 - 4 DEAD // D - 10
                                        "GLO.... SBATD.. HMS.... MMA.... DC?T... "
                                        "GPL.... SSJ.... HBA.... MGR.... DC?T... "
                                        "GPL.... SSJ.... HBA.... MGR.... DC?T...", (struct expectedData) {
                .player = PLAYER_LORD_GODALMING,
                .score = 353,
                .round = 7,

                .location = {PLYMOUTH, SARAJEVO, BARCELONA, GRANADA, CITY_UNKNOWN},
                .history = {{PLYMOUTH,     PLYMOUTH,     LONDON,            PLYMOUTH,          LONDON,           MANCHESTER},
                            {SARAJEVO,     SARAJEVO,     BARCELONA,         MEDITERRANEAN_SEA, MARSEILLES,       CLERMONT_FERRAND},
                            {BARCELONA,    BARCELONA,    MEDITERRANEAN_SEA, MARSEILLES,        CLERMONT_FERRAND, GENEVA},
                            {GRANADA,      GRANADA,      MADRID,            GRANADA,           MADRID,           LISBON},
                            {CITY_UNKNOWN, CITY_UNKNOWN, CITY_UNKNOWN,      BARCELONA,         CITY_UNKNOWN,     BORDEAUX}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER - LIFE_LOSS_HUNTER_ENCOUNTER},

                .connections = connections,
                .nConnections = 3,
        });
    }
    {
        location_t connections[] = {GRANADA, CADIZ, LISBON, SANTANDER, DOUBLE_BACK_1, DOUBLE_BACK_2, DOUBLE_BACK_3,
                                    DOUBLE_BACK_4, DOUBLE_BACK_5, HIDE};
        testFramework("Round 7 Turn 4", "GED.... SGE.... HZU.... MCA.... DCF.V.. "
                                        "GMN.... SCFVD.. HGE.... MLS.... DBOT... "
                                        "GLO.... SMR.... HCF.... MMA.... DTOT... "
                                        "GPL.... SMS.... HMR.... MGR.... DBAT... "
                                        "GLO.... SBATD.. HMS.... MMA.... DSRT... "
                                        "GPL.... SSJ.... HBA.... MGR.... DALT... "
                                        "GPL.... SSJ.... HBA.... MGR.... DMAT... "
                                        "GLO.... SBE.... HMS.... MMATD..", (struct expectedData) {
                .player = PLAYER_DRACULA,
                .score = 353,
                .round = 7,

                .location = {LONDON, BELGRADE, MEDITERRANEAN_SEA, MADRID, MADRID},
                .history = {{LONDON,            PLYMOUTH,  PLYMOUTH,  LONDON,            PLYMOUTH,          LONDON},
                            {BELGRADE,          SARAJEVO,  SARAJEVO,  BARCELONA,         MEDITERRANEAN_SEA, MARSEILLES, CLERMONT_FERRAND},
                            {MEDITERRANEAN_SEA, BARCELONA, BARCELONA, MEDITERRANEAN_SEA, MARSEILLES,        CLERMONT_FERRAND},
                            {MADRID,            GRANADA,   GRANADA,   MADRID,            GRANADA,           MADRID},
                            {MADRID,            ALICANTE,  SARAGOSSA, BARCELONA,         TOULOUSE,          BORDEAUX}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER - LIFE_LOSS_DRACULA_ENCOUNTER,
                           GAME_START_BLOOD_POINTS - 3 * LIFE_LOSS_HUNTER_ENCOUNTER},

                .connections = connections,
                .nConnections = 10,
        });
    }

    ////////////////////////////////////////////////////////////////
    // Extra Cases by Jennifer - 17/8/19
    ////////////////////////////////////////////////////////////////

    /*If there is no legal move possible for Dracula he automagically Teleports to Castle Dracula as his MOVE*/

    {
        location_t connections[] = {TELEPORT};
        testFramework("Test teleport case", "GVA.... SVR.... HAT.... MSA.... DBD.V.. "
                                            "GVA.... SVR.... HAT.... MSA.... DKL.... "
                                            "GVA.... SVR.... HAT.... MSA.... DGA.... "
                                            "GVA.... SVR.... HAT.... MSA.... DBC.... "
                                            "GVA.... SVR.... HAT.... MSA.... DD2.... "
                                            "GVA.... SVR.... HAT.... MSA.... DCD.... "
                                            "GVA.... SVR.... HAT.... MSA.... DHIT... "
                                            "GVA.... SVR.... HAT.... MSA....", (struct expectedData) {
                .player = PLAYER_DRACULA,
                .score = 346,
                .round = 7,

                .location = {VALONA, VARNA, ATHENS, SALONICA, HIDE},
                .history = {{VALONA,   VALONA,         VALONA,        VALONA,    VALONA,   VALONA},
                            {VARNA,    VARNA,          VARNA,         VARNA,     VARNA,    VARNA},
                            {ATHENS,   ATHENS,         ATHENS,        ATHENS,    ATHENS,   ATHENS},
                            {SALONICA, SALONICA,       SALONICA,      SALONICA,  SALONICA, SALONICA},
                            {HIDE,     CASTLE_DRACULA, DOUBLE_BACK_2, BUCHAREST, GALATZ,   KLAUSENBURG}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS + 2 * LIFE_GAIN_CASTLE_DRACULA},

                .connections = connections,
                .nConnections = 1,
        });
    }
    return EXIT_SUCCESS;
}
