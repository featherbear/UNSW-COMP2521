///
/// Created by Jennifer 30/01/19.
///

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "dracula.h"
#include "dracula__ai.h"

// Testing #defines stolen from Andrew
#include "_testing.h"

void register_best_play (char *play, player_message message) {
    // Stub
}

int main (void) {
    puts("Welcome to Dracula AI's testing centre");
    /////////////////////////////////////////////////////////
    DraculaView dv;
    player_message msg[] = {"HAHA"};
    char *pastPlays;
    Queue gen;
    /////////////////////////////////////////////////////////

    pastPlays = "GMN.... SPL.... HAM.... MPA.... DZU.V.. GLV.... SLO.... HNS.... MST....";
    dv = dv_new(pastPlays, msg);
    // Round 1: PLAYER_DRACULA (4) @ ZURICH; 5 locations: {STRASBOURG, GENEVA, MARCEILLES,  MILAN, MUNICH, ZURICH}
    TEST_MSG("Test_1: Testing canMove", dracula_canMove(dv, HIDE) == true);
    TEST_MSG("Test_1: Testing canMove", dracula_canMove(dv, DOUBLE_BACK_1) == true);
    TEST_MSG("Test_1: Testing canMove", dracula_canMove(dv, DOUBLE_BACK_2) == false);


    // Cant get to DB_1, HIDE, ST, ZU, MU, GE,
    // Can get to MI, MR
    A("Test_1: Testing getSafeMoves\n");
    gen = dracula_getSafeMoves(dv);
    printf("Generated Safe location: {");
    while (queue_size(gen) != 0) printf("%s ",location_get_name(queue_de(gen)));
    printf("}\nExpected Safe location: {Milan Marceilles}\n");


    TEST_MSG("Test_1: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_1) == true); // 0 hunters
    TEST_MSG("Test_1: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_2) == true); // 1 hunter
    TEST_MSG("Test_1: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_3) == false); //  3 hunters
    TEST_MSG("Test_1: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_4) == true); // 0 hunters

    TEST_MSG("Test_1: Testing getSafeRegion", dracula_getSafeRegion(dv) == REGION_1);
    TEST_MSG("Test_1: Testing getMoveTowardsRegion", dracula_getMoveTowardsRegion(dv, REGION_1) == MILAN);
    TEST_MSG("Test_1: Testing getDraculaMove", get_dracula_move(dv) == MILAN);

    dv_drop(dv);
    queue_drop(gen);

    /////////////////////////////////////////////////////////
    pastPlays = "GED.... SGE.... HZU.... MCA.... DCF.V.. GMN.... SCFVD.. HGE.... MLS.... DBOT... GLO.... SMR.... HCF.... MMA.... DTOT... GPL.... SMS.... HMR.... MGR.... DBAT... GLO.... SBATD.. HMS.... MMA.... DSRT... GPL.... SSJ.... HBA.... MGR.... DALT... GPL.... SSJ.... HBA.... MGR.... DMAT... GLO.... SBE.... HMS.... MMATD..";
    dv = dv_new(pastPlays, msg);

    // Dracula is at the same place as a hunter (MADRID)
    A("Test_2: Testing getSafeMoves\n");
    gen = dracula_getSafeMoves(dv);
    A("Generated Safe location: {");
    while (queue_size(gen) != 0) A(location_get_name(queue_de(gen)));
    A("}\nExpected Safe location: {Santander}\n");

    TEST_MSG("Test_2: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_1) == true); // 1 hunters
    TEST_MSG("Test_2: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_2) == true); // 0 hunter
    TEST_MSG("Test_2: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_3) == true);// 1 hunters
    TEST_MSG("Test_2: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_4) == false); // 2 hunters
    TEST_MSG("Test_2: Testing getSafeRegion", dracula_getSafeRegion(dv) == REGION_1);
    TEST_MSG("Test_2: Testing getMoveTowardsRegion", dracula_getMoveTowardsRegion(dv, REGION_1) == SANTANDER);
    TEST_MSG("Test_2: Testing getDraculaMove", get_dracula_move(dv) == SANTANDER);

    dv_drop(dv);
    queue_drop(gen);

    /////////////////////////////////////////////////////////
    puts("All tests passed! You are awesome.");
}



/*
 // Stub
    TEST_MSG("Test_1: Testing canMove", dracula_canMove(dv, move) == bool);
    TEST_MSG("Test_1: Testing isSafeMove", dracula_isSafeMove(dv, move) == bool);

    A(Test_1: Testing getSafeMoves);
    Queue gen = dracula_getSafeMoves(dv);
    A("Generated Safe location: {");
    for (queue_size(gen) != 0) A("%s,", location_get_name(queue_de(gen)));
    A("}\nExpected Safe location: { location_t, location_t }\n");

    TEST_MSG("Test_1: Testing isSafeRegion, dracula_isSafeRegion(dv, region) == bool);
    TEST_MSG("Test_1: Testing getSafeRegion, dracula_getSafeRegion(dv) == region_t);
    TEST_MSG("Test_1: Testing getMoveWithinRegion, dracula_getMoveWithinRegion(dv, region) == location_t);
    TEST_MSG("Test_1: Testing getMoveTowardsRegion, dracula_getMoveTowardsRegion(dv, region, dv_get_location(dv, PLAYER_DRACULA)) == location_t);

*/

