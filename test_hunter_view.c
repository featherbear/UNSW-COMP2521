////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// test_hunter_view.c: test the HunterView ADT
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hunter_view.h"
#include "_testing.h"
#include "__pretty.h"

static inline void testFramework(char *title, char *trail, struct expectedData exp) {
    T(title);
    player_message messages[] = {};
    HunterView hv = hv_new(trail, messages);

    TEST(hv_get_round(hv) == exp.round);
    TEST(hv_get_player(hv) == exp.player);
    TEST(hv_get_score(hv) == exp.score);

    {
        TEST(hv_get_health(hv, PLAYER_LORD_GODALMING) == exp.health[PLAYER_LORD_GODALMING]);
        TEST(hv_get_health(hv, PLAYER_DR_SEWARD) == exp.health[PLAYER_DR_SEWARD]);
        TEST(hv_get_health(hv, PLAYER_VAN_HELSING) == exp.health[PLAYER_VAN_HELSING]);
        TEST(hv_get_health(hv, PLAYER_MINA_HARKER) == exp.health[PLAYER_MINA_HARKER]);
        TEST(hv_get_health(hv, PLAYER_DRACULA) == exp.health[PLAYER_DRACULA]);
    }

    {
        TEST(hv_get_location(hv, PLAYER_LORD_GODALMING) == exp.location[PLAYER_LORD_GODALMING]);
        TEST(hv_get_location(hv, PLAYER_DR_SEWARD) == exp.location[PLAYER_DR_SEWARD]);
        TEST(hv_get_location(hv, PLAYER_VAN_HELSING) == exp.location[PLAYER_VAN_HELSING]);
        TEST(hv_get_location(hv, PLAYER_MINA_HARKER) == exp.location[PLAYER_MINA_HARKER]);
        TEST(hv_get_location(hv, PLAYER_DRACULA) == exp.location[PLAYER_DRACULA]);
    }

    location_t history[TRAIL_SIZE];
    {
        A("Check Lord Godalming's move history");
        hv_get_trail(hv, PLAYER_LORD_GODALMING, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_LORD_GODALMING][i]);
        O();
    }
    {
        A("Check Dr Seward's move history");
        hv_get_trail(hv, PLAYER_DR_SEWARD, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_DR_SEWARD][i]);
        O();
    }
    {
        A("Check Van Helsing's move history");
        hv_get_trail(hv, PLAYER_VAN_HELSING, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_VAN_HELSING][i]);
        O();
    }
    {
        A("Check Mina Harker's move history");
        hv_get_trail(hv, PLAYER_MINA_HARKER, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_MINA_HARKER][i]);
        O();
    }
    {
        A("Check Dracula's move history");
        hv_get_trail(hv, PLAYER_DRACULA, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_DRACULA][i]);
        O();
    }

    if (exp.round > 0) {
        A("Check connections for current player");
        size_t nConnections;
        location_t *connections = hv_get_dests_player(hv,  &nConnections, exp.player, true, exp.player != PLAYER_DRACULA, true);
        TEST(arraysEqual(nConnections, connections, exp.nConnections, exp.connections));
        free(connections);
        O();
    }

    hv_drop(hv);
}


int main(void) {

    do {////////////////////////////////////////////////////////////////
        puts ("Test basic empty initialisation");
        char *trail = "";
        player_message messages[] = {};
        HunterView hv = hv_new (trail, messages);

        assert (hv_get_player (hv) == PLAYER_LORD_GODALMING);
        assert (hv_get_round (hv) == 0);
        assert (hv_get_health (hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
        assert (hv_get_health (hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
        assert (hv_get_score (hv) == GAME_START_SCORE);
        assert (hv_get_location (hv, PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);

        puts ("passed");
        hv_drop (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for Dracula trail and basic functions");

        char *trail =
            "GST.... SAO.... HZU.... MBB.... DC?....";
        player_message messages[] = {
            "Hello", "Rubbish", "Stuff", "", "Mwahahah" };
        HunterView hv = hv_new (trail, messages);

        assert (hv_get_player (hv) == PLAYER_LORD_GODALMING);
        assert (hv_get_round (hv) == 1);
        assert (hv_get_location (hv, PLAYER_LORD_GODALMING) == STRASBOURG);
        assert (hv_get_location (hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
        assert (hv_get_location (hv, PLAYER_VAN_HELSING) == ZURICH);
        assert (hv_get_location (hv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
        assert (hv_get_location (hv, PLAYER_DRACULA) == CITY_UNKNOWN);
        assert (hv_get_health (hv, PLAYER_DRACULA) ==
                GAME_START_BLOOD_POINTS);

        puts ("passed");
        hv_drop (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for encountering Dracula and hunter history");
        char *trail =
            "GST.... SAO.... HCD.... MAO.... DGE.... "
            "GGED...";
        player_message messages[] = {
            "Hello", "Rubbish", "Stuff", "", "Mwahahah",
            "Aha!" };
        HunterView hv = hv_new (trail, messages);

        assert (hv_get_location (hv, PLAYER_DRACULA) == GENEVA);
        assert (hv_get_health (hv, PLAYER_LORD_GODALMING) == 5);
        assert (hv_get_health (hv, PLAYER_DRACULA) == 30);
        assert (hv_get_location (hv, PLAYER_LORD_GODALMING) == GENEVA);

        location_t history[TRAIL_SIZE];
        hv_get_trail (hv, PLAYER_DRACULA, history);
        assert (history[0] == GENEVA);
        assert (history[1] == UNKNOWN_LOCATION);

        hv_get_trail (hv, PLAYER_LORD_GODALMING, history);
        assert (history[0] == GENEVA);
        assert (history[1] == STRASBOURG);
        assert (history[2] == UNKNOWN_LOCATION);

        hv_get_trail (hv, PLAYER_DR_SEWARD, history);
        assert (history[0] == ATLANTIC_OCEAN);
        assert (history[1] == UNKNOWN_LOCATION);

        puts ("passed");
        hv_drop (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for Dracula doubling back at sea, "
              "and losing blood points (Hunter View)");
        char *trail =
            "GGE.... SGE.... HGE.... MGE.... DS?.... "
            "GST.... SST.... HST.... MST.... DD1....";
        player_message messages[] = {
            "Hello", "Rubbish", "Stuff", "", "Mwahahah",
            "Aha!", "", "", "", "Back I go" };
        HunterView hv = hv_new (trail, messages);

        assert (hv_get_player (hv) == 0);
        assert (hv_get_location (hv, PLAYER_DRACULA) == DOUBLE_BACK_1);
        assert (hv_get_health (hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);

        location_t history[TRAIL_SIZE];
        hv_get_trail (hv, PLAYER_DRACULA, history);
        assert (history[0] == DOUBLE_BACK_1);
        assert (history[1] == SEA_UNKNOWN);

        puts ("passed");
        hv_drop (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Checking Galatz road connections");
        char *trail = "GGA....";
        player_message messages[] = {"Gone to Galatz"};
        HunterView hv = hv_new (trail, messages);

        size_t n_edges;
        location_t *edges = hv_get_dests_player (
            hv, &n_edges, PLAYER_LORD_GODALMING, true, false, false);
        bool seen[NUM_MAP_LOCATIONS] = {false};
        for (size_t i = 0; i < n_edges; i++)
            seen[edges[i]] = true;

        assert (n_edges == 5);
        assert (seen[GALATZ]);
        assert (seen[CONSTANTA]);
        assert (seen[BUCHAREST]);
        assert (seen[KLAUSENBURG]);
        assert (seen[CASTLE_DRACULA]);

        puts ("passed");
        free (edges);
        hv_drop (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Checking Ionian Sea sea connections");
        char *trail = "GIO....";
        player_message messages[] = {"Sailing the Ionian"};
        HunterView hv = hv_new (trail, messages);

        size_t n_edges;
        location_t *edges = hv_get_dests_player (
            hv, &n_edges, PLAYER_LORD_GODALMING, false, false, true);
        bool seen[NUM_MAP_LOCATIONS] = {false};
        for (size_t i = 0; i < n_edges; i++)
            seen[edges[i]] = true;

        assert (n_edges == 7);
        assert (seen[IONIAN_SEA]);
        assert (seen[BLACK_SEA]);
        assert (seen[ADRIATIC_SEA]);
        assert (seen[TYRRHENIAN_SEA]);
        assert (seen[ATHENS]);
        assert (seen[VALONA]);
        assert (seen[SALONICA]);

        puts ("passed");
        free (edges);
        hv_drop (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Checking Athens rail connections (none)");

        char *trail = "GAT....";
        player_message messages[] = {"Leaving Athens by train"};
        HunterView hv = hv_new (trail, messages);

        size_t n_edges;
        location_t *edges = hv_get_dests_player (
            hv, &n_edges, PLAYER_LORD_GODALMING, false, true, false);

        assert (n_edges == 1);
        assert (edges[0] == ATHENS);

        puts ("passed");
        free (edges);
        hv_drop (hv);
    } while (0);

    ////////////////////////////////////////////////////////////////
    /* Rolling tests */
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
        printf_green("passed\n");

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
        printf_green("passed\n");
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
        printf_green("passed\n");
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
        printf_green("passed\n");
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
        printf_green("passed\n");
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
        printf_green("passed\n");
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
                .history = {{LIVERPOOL, MANCHESTER,        UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {LONDON, PLYMOUTH,             UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
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
        printf_green("passed\n");
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
        printf_green("passed\n");
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
                .history = {{SWANSEA, LIVERPOOL, SWANSEA, IRISH_SEA, LIVERPOOL, MANCHESTER},
                            {LONDON, PLYMOUTH, LONDON, PLYMOUTH, LONDON, PLYMOUTH},
                            {NORTH_SEA, ATLANTIC_OCEAN, NORTH_SEA, ATLANTIC_OCEAN, NORTH_SEA, AMSTERDAM},
                            {ZURICH, FRANKFURT, ZURICH, STRASBOURG, PARIS, UNKNOWN_LOCATION},
                            {CITY_UNKNOWN, CITY_UNKNOWN, CASTLE_DRACULA, CITY_UNKNOWN, CITY_UNKNOWN, UNKNOWN_LOCATION}},
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
        printf_green("passed\n");
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
                .history = {{PLYMOUTH, PLYMOUTH, LONDON,PLYMOUTH, LONDON, MANCHESTER},
                            {SARAJEVO, SARAJEVO, BARCELONA, MEDITERRANEAN_SEA, MARSEILLES, CLERMONT_FERRAND},
                            {BARCELONA, BARCELONA, MEDITERRANEAN_SEA, MARSEILLES, CLERMONT_FERRAND, GENEVA},
                            {GRANADA, GRANADA, MADRID, GRANADA, MADRID, LISBON},
                            {CITY_UNKNOWN, CITY_UNKNOWN, CITY_UNKNOWN, BARCELONA, CITY_UNKNOWN, BORDEAUX}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER - LIFE_LOSS_HUNTER_ENCOUNTER},

                .connections = connections,
                .nConnections = 3,
        });
        printf_green("passed\n");
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
                .history = {{LONDON, PLYMOUTH,  PLYMOUTH,  LONDON, PLYMOUTH,        LONDON},
                            {BELGRADE, SARAJEVO, SARAJEVO, BARCELONA,        MEDITERRANEAN_SEA, MARSEILLES, CLERMONT_FERRAND},
                            {MEDITERRANEAN_SEA, BARCELONA, BARCELONA, MEDITERRANEAN_SEA, MARSEILLES, CLERMONT_FERRAND},
                            {MADRID, GRANADA, GRANADA, MADRID, GRANADA, MADRID},
                            {MADRID, ALICANTE,  SARAGOSSA, BARCELONA, TOULOUSE,         BORDEAUX}},
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

/*
 {
        location_t connections[] = {PLYMOUTH, ENGLISH_CHANNEL, LONDON};
        testFramework("????????", "", (struct expectedData) {
                .player = ???,
                .score = ???,
                .round = ???,

                .location = {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION,             UNKNOWN_LOCATION},
                .history = {{UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS ,
                           GAME_START_BLOOD_POINTS },

                .connections = connections,
                .nConnections = ???,
        });
    }
*/


    // { /* Whenever a Hunter is reduced to 0 or less life points they are instantly automagically teleported to the Hospital of St Joseph and St Mary */
    // // NOTE: Not sure if MINA is at Hospital yet.. But she has lost all her health so she should be there? But it's not included within the trail.. please double check
    //     location_t connections[] = {ZAGREB, SZEGED, BELGRADE, SARAJEVO, HOSPITAL};
    //     testFramework("Testing Teleportation", "", (struct expectedData) {
    //             .player = PLAYER_MINA_HARKER,
    //             .score = 366 - 4, // Uhhh not sure if right ..
    //             .round = 5,

    //             .location = {VALONA, VARNA, ATHENS, ST_JOSEPH_AND_ST_MARYS, MADRID},
    //             .history = {{VALONA, VALONA, VALONA, VALONA, VALONA, UNKNOWN_LOCATION},
    //                         {VARNA, VARNA , VARNA, VARNA , VARNA, UNKNOWN_LOCATION },
    //                         {ATHENS, ATHENS, ATHENS, ATHENS, ATHENS,UNKNOWN_LOCATION},
    //                         {SANTANDER, SARAGOSSA, ALICANTE, GRANADA, HOSPITAL, UNKNOWN_LOCATION},
    //                         {SARAGOSSA, ALICANTE, GRANADA, MADRID, UNKNOWN_LOCATION. UNKNOWN_LOCATION}},
    //             .health = {GAME_START_HUNTER_LIFE_POINTS,
    //                       GAME_START_HUNTER_LIFE_POINTS,
    //                       GAME_START_HUNTER_LIFE_POINTS,
    //                       GAME_START_HUNTER_LIFE_POINTS ,
    //                       GAME_START_BLOOD_POINTS },

    //             .connections = connections,
    //             .nConnections = 4,
    //     });
    // }
}
