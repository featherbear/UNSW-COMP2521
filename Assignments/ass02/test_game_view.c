////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// test_game_view.c: test the GameView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_view.h"
#include "_testing.h"
#include "__pretty.h"

static inline void testFramework(char *title, char *trail, struct expectedData exp) {
    T(title);
    player_message messages[] = {};
    GameView gv = gv_new(trail, messages);

    TEST(gv_get_round(gv) == exp.round);
    TEST(gv_get_player(gv) == exp.player);
    TEST(gv_get_score(gv) == exp.score);

    {
        TEST(gv_get_health(gv, PLAYER_LORD_GODALMING) == exp.health[PLAYER_LORD_GODALMING]);
        TEST(gv_get_health(gv, PLAYER_DR_SEWARD) == exp.health[PLAYER_DR_SEWARD]);
        TEST(gv_get_health(gv, PLAYER_VAN_HELSING) == exp.health[PLAYER_VAN_HELSING]);
        TEST(gv_get_health(gv, PLAYER_MINA_HARKER) == exp.health[PLAYER_MINA_HARKER]);
        TEST(gv_get_health(gv, PLAYER_DRACULA) == exp.health[PLAYER_DRACULA]);
    }

    {
        TEST(gv_get_location(gv, PLAYER_LORD_GODALMING) == exp.location[PLAYER_LORD_GODALMING]);
        TEST(gv_get_location(gv, PLAYER_DR_SEWARD) == exp.location[PLAYER_DR_SEWARD]);
        TEST(gv_get_location(gv, PLAYER_VAN_HELSING) == exp.location[PLAYER_VAN_HELSING]);
        TEST(gv_get_location(gv, PLAYER_MINA_HARKER) == exp.location[PLAYER_MINA_HARKER]);
        TEST(gv_get_location(gv, PLAYER_DRACULA) == exp.location[PLAYER_DRACULA]);
    }

    location_t history[TRAIL_SIZE];
    {
        A("Check Lord Godalming's move history");
        gv_get_history(gv, PLAYER_LORD_GODALMING, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_LORD_GODALMING][i]);
        O();
    }
    {
        A("Check Dr Seward's move history");
        gv_get_history(gv, PLAYER_DR_SEWARD, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_DR_SEWARD][i]);
        O();
    }
    {
        A("Check Van Helsing's move history");
        gv_get_history(gv, PLAYER_VAN_HELSING, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_VAN_HELSING][i]);
        O();
    }
    {
        A("Check Mina Harker's move history");
        gv_get_history(gv, PLAYER_MINA_HARKER, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_MINA_HARKER][i]);
        O();
    }
    {
        A("Check Dracula's move history");
        gv_get_history(gv, PLAYER_DRACULA, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_DRACULA][i]);
        O();
    }

    if (exp.round > 0) {
        A("Check connections for current player");
        size_t nConnections;
        location_t *connections = gv_get_connections(gv, &nConnections, gv_get_location(gv, exp.player), exp.player,
                                                     exp.round,
                                                     true, exp.player != PLAYER_DRACULA, true);
        assert(arraysEqual(nConnections, connections, exp.nConnections, exp.connections));
        free(connections);
        O();
    }

    gv_drop(gv);
    printf_green("passed\n");
}


int main(void) {
    do {////////////////////////////////////////////////////////////////
        T("Test basic empty initialisation");

        char *trail = "";
        player_message messages[] = {};
        GameView gv = gv_new(trail, messages);

        TEST(gv_get_player(gv) == PLAYER_LORD_GODALMING);
        TEST(gv_get_round(gv) == 0);
        TEST(gv_get_health(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
        TEST(gv_get_health(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
        TEST(gv_get_score(gv) == GAME_START_SCORE);
        TEST(gv_get_location(gv, PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);

        gv_drop(gv);
    } while (0);

    do {////////////////////////////////////////////////////////////////
        T("Test for Dracula trail and basic functions");

        char *trail = "GST.... SAO.... HZU.... MBB.... DC?....";
        player_message messages[] = {"Hello", "Rubbish", "Stuff", "", "Mwahahah"};
        GameView gv = gv_new(trail, messages);

        TEST(gv_get_player(gv) == PLAYER_LORD_GODALMING);
        TEST(gv_get_round(gv) == 1);

        TEST(gv_get_location(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
        TEST(gv_get_location(gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
        TEST(gv_get_location(gv, PLAYER_VAN_HELSING) == ZURICH);
        TEST(gv_get_location(gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
        TEST(gv_get_location(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
        TEST(gv_get_health(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);


        gv_drop(gv);
    } while (0);

    do {////////////////////////////////////////////////////////////////
        T("Test for encountering Dracula and hunter history");

        char *trail =
                "GST.... SAO.... HCD.... MAO.... DGE.... "
                "GGED...";
        player_message messages[] = {
                "Hello", "Rubbish", "Stuff", "", "Mwahahah",
                "Aha!"};
        GameView gv = gv_new(trail, messages);

        TEST(gv_get_location(gv, PLAYER_DRACULA) == GENEVA);
        TEST(gv_get_health(gv, PLAYER_LORD_GODALMING) == 5);
        TEST(gv_get_health(gv, PLAYER_DRACULA) == 30);
        TEST(gv_get_location(gv, PLAYER_LORD_GODALMING) == GENEVA);

        location_t history[TRAIL_SIZE];

        A("Check Dracula's move history");
        gv_get_history(gv, PLAYER_DRACULA, history);
        assert(history[0] == GENEVA);
        assert(history[1] == UNKNOWN_LOCATION);
        O();

        A("Check Lord Godalming's move history");
        gv_get_history(gv, PLAYER_LORD_GODALMING, history);
        assert(history[0] == GENEVA);
        assert(history[1] == STRASBOURG);
        assert(history[2] == UNKNOWN_LOCATION);
        O();

        A("Check Dr Seward's move history");
        gv_get_history(gv, PLAYER_DR_SEWARD, history);
        assert(history[0] == ATLANTIC_OCEAN);
        assert(history[1] == UNKNOWN_LOCATION);
        O();


        gv_drop(gv);
    } while (0);



    // TODO test for dead player
    // location, health, moves???

    // TODO  test for traps

    do {////////////////////////////////////////////////////////////////
        T("Test for Dracula doubling back at sea, "
          "and losing blood points (Hunter View)");

        char *trail =
                "GGE.... SGE.... HGE.... MGE.... DS?.... "
                "GST.... SST.... HST.... MST.... DD1....";
        player_message messages[] = {
                "Hello", "Rubbish", "Stuff", "", "Mwahahah",
                "Aha!", "", "", "", "Back I go"};
        GameView gv = gv_new(trail, messages);

        TEST(gv_get_player(gv) == 0);
        TEST(gv_get_health(gv, PLAYER_DRACULA) ==
             GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
        TEST(gv_get_location(gv, PLAYER_DRACULA) == DOUBLE_BACK_1);

        location_t history[TRAIL_SIZE];

        A("Check Dracula's move history");
        gv_get_history(gv, PLAYER_DRACULA, history);
        assert(history[0] == DOUBLE_BACK_1);
        assert(history[1] == SEA_UNKNOWN);
        O();


        gv_drop(gv);

    } while (0);

    do {////////////////////////////////////////////////////////////////
        T("Test for Dracula doubling back at sea, "
          "and losing blood points (Dracula View)");

        char *trail =
                "GGE.... SGE.... HGE.... MGE.... DEC.... "
                "GST.... SST.... HST.... MST.... DD1....";
        player_message messages[] = {
                "Hello", "Rubbish", "Stuff", "", "Mwahahah",
                "Aha!", "", "", "", "Back I go"};
        GameView gv = gv_new(trail, messages);

        TEST(gv_get_player(gv) == 0);
        TEST(gv_get_health(gv, PLAYER_DRACULA) ==
             GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
        TEST(gv_get_location(gv, PLAYER_DRACULA) == DOUBLE_BACK_1);

        location_t history[TRAIL_SIZE];

        A("Check Dracula's move history");
        gv_get_history(gv, PLAYER_DRACULA, history);
        assert(history[0] == DOUBLE_BACK_1);
        assert(history[1] == ENGLISH_CHANNEL);
        O();


        gv_drop(gv);
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
        location_t connections[] = {STRASBOURG, GENEVA, MILAN, MUNICH, MARSEILLES, ZURICH};
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
        // J: Changed this
        location_t connections[] = {CONSTANTA, BELGRADE, SOFIA, KLAUSENBURG, CASTLE_DRACULA, GALATZ, BUCHAREST};
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
        // J: Deleted the 6th location in Drac's trail & updated nCons
        location_t connections[] = {GRANADA, CADIZ, LISBON, SANTANDER, ALICANTE, SARAGOSSA, BARCELONA,
                                    TOULOUSE, MADRID};
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
                            {BELGRADE,          SARAJEVO,  SARAJEVO,  BARCELONA,         MEDITERRANEAN_SEA, MARSEILLES},
                            {MEDITERRANEAN_SEA, BARCELONA, BARCELONA, MEDITERRANEAN_SEA, MARSEILLES,        CLERMONT_FERRAND},
                            {MADRID,            GRANADA,   GRANADA,   MADRID,            GRANADA,           MADRID},
                            {MADRID,            ALICANTE,  SARAGOSSA, BARCELONA,         TOULOUSE,          BORDEAUX}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER - LIFE_LOSS_DRACULA_ENCOUNTER,
                           GAME_START_BLOOD_POINTS - 3 * LIFE_LOSS_HUNTER_ENCOUNTER},

                .connections = connections,
                .nConnections = 9,
        });
    }

    ////////////////////////////////////////////////////////////////
    // Extra Cases by Jennifer - 17/8/19
    ////////////////////////////////////////////////////////////////
    {
        location_t connections[] = {CASTLE_DRACULA};
        testFramework("Test teleport case", "GMA.... SMN.... HKL.... MKL.... DGA.V.. "
                                            "GAL.... SED.... HKL.... MKL.... DCDT... "
                                            "GMS.... SNS.... HKL.... MKL.... DKLT... "
                                            "GMR.... SHA.... HKL.... MKL.... DD2T... "
                                            "GGO.... SBR.... HKL.... MKL.... DHIT... "
                                            "GVE.... SPR.... HKL.... MKL....", (struct expectedData) {
                .player = PLAYER_DRACULA,
                .score = 361,
                .round = 5,

                .location = {VENICE, PRAGUE, KLAUSENBURG, KLAUSENBURG, HIDE},
                .history = {{VENICE,      GENOA,         MARSEILLES,  MEDITERRANEAN_SEA, ALICANTE,    MADRID},
                            {PRAGUE,      BERLIN,        HAMBURG,     NORTH_SEA,         EDINBURGH,   MANCHESTER},
                            {KLAUSENBURG, KLAUSENBURG,   KLAUSENBURG, KLAUSENBURG,       KLAUSENBURG, KLAUSENBURG},
                            {KLAUSENBURG, KLAUSENBURG,   KLAUSENBURG, KLAUSENBURG,       KLAUSENBURG, KLAUSENBURG},
                            {HIDE,        DOUBLE_BACK_2, KLAUSENBURG, CASTLE_DRACULA,    GALATZ,      UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS + 3 * LIFE_GAIN_CASTLE_DRACULA},
                .connections = connections,
                .nConnections = 1,
        });
    }

    {
        location_t connections[] = {BUDAPEST, KLAUSENBURG, BELGRADE, ZAGREB, SZEGED};
        testFramework("Test no-hospital", "GVA.... SVR.... HAT.... MSA.... DSZ.V.. "
                                          "GVA.... SVR.... HAT.... MSA....", (struct expectedData) {
                              .player = PLAYER_DRACULA,
                              .score = 365,
                              .round = 1,

                              .location = {VALONA, VARNA, ATHENS, SALONICA, SZEGED},
                              .history = {{VALONA,   VALONA,           UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                                          {VARNA,    VARNA,            UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                                          {ATHENS,   ATHENS,           UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                                          {SALONICA, SALONICA,         UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                                          {SZEGED,   UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}
                              },

                              .health = {GAME_START_HUNTER_LIFE_POINTS,
                                         GAME_START_HUNTER_LIFE_POINTS,
                                         GAME_START_HUNTER_LIFE_POINTS,
                                         GAME_START_HUNTER_LIFE_POINTS,
                                         GAME_START_BLOOD_POINTS},

                              .connections = connections,
                              .nConnections = 5,
                      }

        );
    }

    {
        location_t connections[] = {ZAGREB, SZEGED, BELGRADE, SARAJEVO, HOSPITAL_LOCATION};
        testFramework("Test hunter death teleportation", "GVA.... SVR.... HAT.... MSN.... DSRT... "
                                                         "GVA.... SVR.... HAT.... MSRTD.. DALT... "
                                                         "GVA.... SVR.... HAT.... MALTD.. DGRT... "
                                                         "GVA.... SVR.... HAT....", (struct expectedData) {
                              // Poor MINA keeps following Dracula losing health :'(
                              .player = PLAYER_MINA_HARKER,
                              .score = 357,
                              .round = 3,
                              .location = {VALONA, VARNA, ATHENS, HOSPITAL_LOCATION, GRANADA},
                              .history = {{VALONA,   VALONA,    VALONA,    VALONA,           UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                                          {VARNA,    VARNA,     VARNA,     VARNA,            UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                                          {ATHENS,   ATHENS,    ATHENS,    ATHENS,           UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                                          {ALICANTE, SARAGOSSA, SANTANDER, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                                          {GRANADA,  ALICANTE,  SARAGOSSA, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}
                              },

                              .health = {GAME_START_HUNTER_LIFE_POINTS,
                                         GAME_START_HUNTER_LIFE_POINTS,
                                         GAME_START_HUNTER_LIFE_POINTS,
                                         0,
                                         GAME_START_BLOOD_POINTS - 2 * LIFE_LOSS_HUNTER_ENCOUNTER},

                              .connections = connections,
                              .nConnections = 5,
                      }

        );
    }

    {
        location_t connections[] = {FLORENCE, VENICE, TYRRHENIAN_SEA, GENOA};
        testFramework("Test no-doubleback", "GVA.... SVR.... HAT.... MSA.... DLE.... "
                                            "GVA.... SVR.... HAT.... MSA.... DPAT... "
                                            "GVA.... SVR.... HAT.... MSA.... DCFT... "
                                            "GVA.... SVR.... HAT.... MSA.... DMRT... "
                                            "GVA.... SVR.... HAT.... MSA.... DGOT... "
                                            "GVA.... SVR.... HAT.... MSA.... DMIT... "
                                            "GVA.... SVR.... HAT.... MSA.... DZUT... "
                                            "GVA.... SVR.... HAT.... MSA.... DD3T... "
                                            "GVA.... SVR.... HAT.... MSA....", (struct expectedData) {
                .player = PLAYER_DRACULA,
                .score = 358,
                .round = 8,

                .location = {VALONA, VARNA, ATHENS, SALONICA, DOUBLE_BACK_3},
                .history = {{VALONA,        VALONA,   VALONA,   VALONA,   VALONA,     VALONA},
                            {VARNA,         VARNA,    VARNA,    VARNA,    VARNA,      VARNA},
                            {ATHENS,        ATHENS,   ATHENS,   ATHENS,   ATHENS,     ATHENS},
                            {SALONICA,      SALONICA, SALONICA, SALONICA, SALONICA,   SALONICA},
                            {DOUBLE_BACK_3, ZURICH,   MILAN,    GENOA,    MARSEILLES, CLERMONT_FERRAND}
                },
                .health = {
                        GAME_START_HUNTER_LIFE_POINTS,
                        GAME_START_HUNTER_LIFE_POINTS,
                        GAME_START_HUNTER_LIFE_POINTS,
                        GAME_START_HUNTER_LIFE_POINTS,
                        GAME_START_BLOOD_POINTS
                },

                .connections = connections,
                .nConnections = 4,
        });
    }


    return EXIT_SUCCESS;


/*
 // STUB
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

}
