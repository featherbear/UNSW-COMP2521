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
#include "__pretty.h"


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

    {
        TEST(dv_get_location(dv, PLAYER_LORD_GODALMING) == exp.location[PLAYER_LORD_GODALMING]);
        TEST(dv_get_location(dv, PLAYER_DR_SEWARD) == exp.location[PLAYER_DR_SEWARD]);
        TEST(dv_get_location(dv, PLAYER_VAN_HELSING) == exp.location[PLAYER_VAN_HELSING]);
        TEST(dv_get_location(dv, PLAYER_MINA_HARKER) == exp.location[PLAYER_MINA_HARKER]);
        TEST(dv_get_location(dv, PLAYER_DRACULA) == exp.location[PLAYER_DRACULA]);
    }

    {
        location_t start;
        location_t end;
        {
            A("Check Lord Godalming's player move");
            dv_get_player_move(dv, PLAYER_LORD_GODALMING, &start, &end);
            assert(exp.history[PLAYER_LORD_GODALMING][1] == start);
            assert(exp.history[PLAYER_LORD_GODALMING][0] == end);
            O();
        }
        {
            A("Check Dr Seward's player move");
            dv_get_player_move(dv, PLAYER_DR_SEWARD, &start, &end);
            assert(exp.history[PLAYER_DR_SEWARD][1] == start);
            assert(exp.history[PLAYER_DR_SEWARD][0] == end);
            O();
        }
        {
            A("Check Van Helsing's player move");
            dv_get_player_move(dv, PLAYER_VAN_HELSING, &start, &end);
            assert(exp.history[PLAYER_VAN_HELSING][1] == start);
            assert(exp.history[PLAYER_VAN_HELSING][0] == end);
            O();
        }
        {
            A("Check Mina Harker's player move");
            dv_get_player_move(dv, PLAYER_MINA_HARKER, &start, &end);
            assert(exp.history[PLAYER_MINA_HARKER][1] == start);
            assert(exp.history[PLAYER_MINA_HARKER][0] == end);
            O();
        }
        {
            A("Check Dracula's player move");
            dv_get_player_move(dv, PLAYER_DRACULA, &start, &end);
            printf("%d -> %d\n", exp.history[PLAYER_DRACULA][1], exp.history[PLAYER_DRACULA][1]);
            printf("%d -> %d\n", start, end);

            assert(exp.history[PLAYER_DRACULA][1] == start);
            assert(exp.history[PLAYER_DRACULA][0] == end);
            O();
        }
    }

    // TODO: dc_get_locale_info(dv, where, *n_traps, *n_vamps);

    location_t history[TRAIL_SIZE];
    {
        A("Check Lord Godalming's trail");
        dv_get_trail(dv, PLAYER_LORD_GODALMING, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_LORD_GODALMING][i]);
        O();
    }
    {
        A("Check Dr Seward's trail");
        dv_get_trail(dv, PLAYER_DR_SEWARD, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_DR_SEWARD][i]);
        O();
    }
    {
        A("Check Van Helsing's trail");
        dv_get_trail(dv, PLAYER_VAN_HELSING, history);
        for (size_t i = 0; i < TRAIL_SIZE; i++) assert(history[i] == exp.history[PLAYER_VAN_HELSING][i]);
        O();
    }
    {
        A("Check Mina Harker's trail");
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
        size_t nConnections;

        {
            location_t *connections;

            A("dv_get_dests | 1");
            connections = dv_get_dests(dv, &nConnections, true, true);
            assert(arraysEqual(nConnections, connections, exp.nConnections, exp.connections));
            free(connections);
            O();

            A("dv_get_dests | 2");
            connections = dv_get_dests(dv, &nConnections, true, true);
            assert(arraysEqual(nConnections, connections, exp.nConnections, exp.connections));
            free(connections);
            O();
        }
    }

    dv_drop(dv);
}

int main(void) {


    do {////////////////////////////////////////////////////////////////
        puts("Test basic empty initialisation");
        char *trail = "";
        player_message messages[] = {};
        DraculaView dv = dv_new(trail, messages);

        assert(dv_get_round(dv) == 0);
        assert(dv_get_health(dv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
        assert(dv_get_health(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
        assert(dv_get_score(dv) == GAME_START_SCORE);
        assert(dv_get_location(dv, PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);

        printf_green("Passed\n");
        dv_drop(dv);
    } while (0);



    //////////////////////////////////////////////////////////////

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


    {   // J: Changed this.
        // trail = {BUCHAREST, KLAUSENBURG, CASTLE_DRACULA, GALATZ, GALATZ, UNKNOWN_LOCATION}
        location_t connections[] = {CONSTANTA, BELGRADE, SOFIA, BUCHAREST, KLAUSENBURG, CASTLE_DRACULA, GALATZ};
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
                            {BUCHAREST,  KLAUSENBURG,    CASTLE_DRACULA, GALATZ,         GALATZ,     UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS + LIFE_GAIN_CASTLE_DRACULA},

                .connections = connections,
                .nConnections = 7,
        });
    }

    {
        // J: Changed this.
        // Past Trail: {MADRID, ALICANTE, SARAGOSSA, BARCELONA, TOULOUSE, BORDEAUX}
        location_t connections[] = {GRANADA, CADIZ, LISBON, SANTANDER, ALICANTE, SARAGOSSA, BARCELONA, TOULOUSE, DORDEAUX, MADRID};
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

                .location = {VALONA, VARNA, ATHENS, SALONICA, CASTLE_DRACULA},
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

    // J: Added a new test
    { /* Dracula cannot make a DOUBLE_BACK move of 'n' if he hasn't had 'n' moves */
        location_t connections[] = {PARIS, LE_HAVRE, NANTES, BORDEAUX, CLERMONT_FERRAND, BAY_OF_BISCAY};
        testFramework("Test Double Backs", "GVA.... SVR.... HAT.... MSA.... DLE.V.. GVA.... SVR.... HAT.... MSA.... DPAT... GVA.... SVR.... HAT.... MSA.... DNAT... GVA.... SVR.... HAT.... MSA....", (struct expectedData) {
                .player = PLAYER_DRACULA,
                .score = 366 - 3,
                .round = 3,

                .location = {VALONA, VARNA, ATHENS, SALONICA,             UNKNOWN_LOCATION},
                .history = {{VALONA, VALONA, VALONA, VALONA, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {VARNA, VARNA, VARNA, VARNA, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {ATHENS, ATHENS, ATHENS, ATHENS, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {SALONICA, SALONICA, SALONICA, SALONICA, UNKNOWN_LOCATION, UNKNOWN_LOCATION},
                            {NANTES, PARIS, LE_HAVRE, UNKNOWN_LOCATION, UNKNOWN_LOCATION, UNKNOWN_LOCATION}},
                .health = {GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_HUNTER_LIFE_POINTS,
                           GAME_START_BLOOD_POINTS },

                .connections = connections,
                .nConnections = 7,
        });
    }
    return EXIT_SUCCESS;
}
