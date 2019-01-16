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

//#define tTEST(x) printf("  assert [" #x); TEST(x); puts("] OK");


static bool itemInArray(int needle, int *haystack, size_t n_haystack) {
    for (size_t i = 0; i < n_haystack; i++) {
        if (haystack[i] == needle) return true;
    }
    return false;
}

static bool arraysEqual(size_t nA, int *A, size_t nB, int *B) {
    if (nA != nB) return false;

    for (size_t i = 0; i < nA; i++) {
        if (!itemInArray(A[i], B, nB)) return false;
    }

    return true;
}

static struct expectedData {
    round_t round;
    enum player player;
    int score;
    int health[NUM_PLAYERS];
    location_t location[NUM_PLAYERS];
    location_t history[NUM_PLAYERS][TRAIL_SIZE];
    size_t nConnections;
    location_t *connections
} expectedData;

static inline void testFramework(char *title, char *trail, expectedData exp) {
    T(title);
    player_message messages[] = {};
    GameView gv = gv_new(trail, messages);

    TEST(gv_get_round(gv) == exp.round);
    TEST(gv_get_player(gv) == exp.player);
    TEST(gv_get_score(gv) == exp.score);

    {
        TEST(gv_get_health(gv, PLAYER_LORD_GODALMING) == exp.health[PLAYER_LORD_GODALMING]));
        TEST(gv_get_health(gv, PLAYER_DR_SEWARD) == exp.health[PLAYER_DR_SEWARD]));
        TEST(gv_get_health(gv, PLAYER_VAN_HELSING) == exp.health[PLAYER_VAN_HELSING]));
        TEST(gv_get_health(gv, PLAYER_MINA_HARKER) == exp.health[PLAYER_MINA_HARKER]));
        TEST(gv_get_health(gv, PLAYER_DRACULA) == exp.health[PLAYER_DRACULA]));
    }
    {
        TEST(gv_get_location(gv, PLAYER_LORD_GODALMING) == exp.location[PLAYER_LORD_GODALMING]));
        TEST(gv_get_location(gv, PLAYER_DR_SEWARD) == exp.location[PLAYER_DR_SEWARD]));
        TEST(gv_get_location(gv, PLAYER_VAN_HELSING) == exp.location[PLAYER_VAN_HELSING]));
        TEST(gv_get_location(gv, PLAYER_MINA_HARKER) == exp.location[PLAYER_MINA_HARKER]));
        TEST(gv_get_location(gv, PLAYER_DRACULA) == exp.location[PLAYER_DRACULA]));
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
        location_t *connections = gv_get_connections(gv, &nConnections, gv_get_location(gv, exp.player), exp.round,
                                                     true, exp.player != PLAYER_DRACULA, true);
        TEST(arraysEqual(nConnections, connections, exp.nConnections, exp.connections));
        O();
    }

    gv_drop(gv);
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
        puts("passed");
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

        puts("passed");
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

        puts("passed");
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

        puts("passed");
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

        puts("passed");
        gv_drop(gv);
    } while (0);

//return -1;

    /*do {////////////////////////////////////////////////////////////////
        puts("Test for connections");
        char *trail = "";
        player_message messages[] = {};
        GameView gv = gv_new(trail, messages);

        do {
            puts("Checking Galatz road connections");
            size_t n_edges;
            location_t *edges = gv_get_connections(
                    gv, &n_edges,
                    GALATZ, PLAYER_LORD_GODALMING, 0,
                    true, false, false
            );

            bool seen[NUM_MAP_LOCATIONS] = {false};
            for (size_t i = 0; i < n_edges; i++)
                seen[edges[i]] = true;

            TEST(n_edges == 5);
            TEST(seen[GALATZ]);
            TEST(seen[CONSTANTA]);
            TEST(seen[BUCHAREST]);
            TEST(seen[KLAUSENBURG]);
            TEST(seen[CASTLE_DRACULA]);

            free(edges);
        } while (0);

        do {
            puts("Checking Ionian Sea sea connections");
            size_t n_edges;
            location_t *edges = gv_get_connections(
                    gv, &n_edges,
                    IONIAN_SEA, PLAYER_LORD_GODALMING, 0,
                    false, false, true
            );

            bool seen[NUM_MAP_LOCATIONS] = {false};
            for (size_t i = 0; i < n_edges; i++)
                seen[edges[i]] = true;

            TEST(n_edges == 7);
            TEST(seen[IONIAN_SEA]);
            TEST(seen[BLACK_SEA]);
            TEST(seen[ADRIATIC_SEA]);
            TEST(seen[TYRRHENIAN_SEA]);
            TEST(seen[ATHENS]);
            TEST(seen[VALONA]);
            TEST(seen[SALONICA]);

            free(edges);
        } while (0);

        do {
            puts("Checking Athens rail connections (none)");
            size_t n_edges;
            location_t *edges = gv_get_connections(
                    gv, &n_edges,
                    ATHENS, PLAYER_LORD_GODALMING, 0,
                    false, true, false
            );

            TEST(n_edges == 1);
            TEST(edges[0] == ATHENS);

            free(edges);
        } while (0);

        puts("passed");
        gv_drop(gv);
    } while (0);*/

    /* Rolling tests */
    testFramework("Round 0 Turn 0", "", {
            .player = PLAYER_LORD_GODALMING,
            .score = 366,
            .round = 0,

            .location = {UNKNOWN_LOCATION},
            .history = {{UNKNOWN_LOCATION},
                        {UNKNOWN_LOCATION},
                        {UNKNOWN_LOCATION},
                        {UNKNOWN_LOCATION},
                        {UNKNOWN_LOCATION}},
            .health = {GAME_START_HUNTER_LIFE_POINTS,
                       GAME_START_HUNTER_LIFE_POINTS,
                       GAME_START_HUNTER_LIFE_POINTS,
                       GAME_START_HUNTER_LIFE_POINTS,
                       GAME_START_BLOOD_POINTS},

            .connections = NULL,
            .nConnections = -1,
    });

    testFramework("Round 0 Turn 1", "GMN....", {
            .player = PLAYER_DR_SEWARD,
            .score = 366,
            .round = 0,

            .location = {MANCHESTER, UNKNOWN_LOCATION},
            .history = {{UNKNOWN_LOCATION},
                        {UNKNOWN_LOCATION},
                        {UNKNOWN_LOCATION},
                        {UNKNOWN_LOCATION},
                        {UNKNOWN_LOCATION}},
            .health = {GAME_START_HUNTER_LIFE_POINTS,
                       GAME_START_HUNTER_LIFE_POINTS,
                       GAME_START_HUNTER_LIFE_POINTS,
                       GAME_START_HUNTER_LIFE_POINTS,
                       GAME_START_BLOOD_POINTS},

            .connections = NULL,
            .nConnections = -1,
    });

    return EXIT_SUCCESS;
}
