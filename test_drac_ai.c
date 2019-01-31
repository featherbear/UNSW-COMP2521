///
/// Created by Jennifer 30/01/19.
///

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "dracula.h"

// Testing #defines stolen from Andrew
#define TEST(ast)          A(#ast); assert(ast); O();
#define TEST_MSG(msg, ast) A(msg);  assert(ast); O();

#define O()  printf(" [OK]\n");
#define T(m) printf("\n%s ::\n", m);
#define A(m) printf("  %s", m);

void register_best_play (char *play, player_message message) {

}

int main (void) {
    printf("Welcome to Dracula AI's testing centre");
    /////////////////////////////////////////////////////////
    DraculaView dv;
    player_message msg[] = {"HAHA"};
    char *pastPlays;
    /////////////////////////////////////////////////////////

    pastPlays = "GMN.... SPL.... HAM.... MPA.... DZU.V.. GLV.... SLO.... HNS.... MST....";
    dv = dv_new(pastPlays, msg);
    // Round 1: PLAYER_DRACULA (4) @ ZURICH; 5 locations: {STRASBOURG, GENEVA, MARCEILLES,  MILAN, MUNICH, ZURICH}
    TEST_MSG("Test_1: Testing canMove", dracula_canMove(dv, HIDE) == true);
    TEST_MSG("Test_1: Testing canMove", dracula_canMove(dv, DOUBLE_BACK_1) == true);
    TEST_MSG("Test_1: Testing canMove", dracula_canMove(dv, DOUBLE_BACK_2) == false);


    //Cant get to DB_1, HIDE, ST, ZU, MU, GE,
    //Can get to MI, MR
    // TEST_MSG("Test_1: Testing isSafeMove", dracula_isSafeMove(dv, DOUBLE_BACK_1) == false);
    // TEST_MSG("Test_1: Testing isSafeMove", dracula_isSafeMove(dv, HIDE) == false);
    // TEST_MSG("Test_1: Testing isSafeMove", dracula_isSafeMove(dv, STRASBOURG) == false);
    // TEST_MSG("Test_1: Testing isSafeMove", dracula_isSafeMove(dv, ZURICH) == false);
    // TEST_MSG("Test_1: Testing isSafeMove", dracula_isSafeMove(dv, MUNICH) == false);
    // TEST_MSG("Test_1: Testing isSafeMove", dracula_isSafeMove(dv, GENEVA) == false);

    // TEST_MSG("Test_1: Testing isSafeMove", dracula_isSafeMove(dv, MILAN) == true);
    // TEST_MSG("Test_1: Testing isSafeMove", dracula_isSafeMove(dv, MARSEILLES) == true);

    /* Want to test the static function: Queue dracula_getSafeMoves(DraculaView dv);*/
    A("Test_1: Testing getSafeMoves");
    Queue gen = dracula_getSafeMoves(dv);
    A("Generated Safe location: {");
    while (queue_size(gen) != 0) A(location_get_name(queue_de(gen)));
    A("}\nExpected Safe location: { Milan, Marceilles }\n");

    // TEST_MSG("Test_1: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_1) == true); // no hunters
    // TEST_MSG("Test_1: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_2) == true); // 1 hunter
    // TEST_MSG("Test_1: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_3) == true); //  hunters
    // TEST_MSG("Test_1: Testing isSafeRegion", dracula_isSafeRegion(dv, REGION_4) == false); // 3 hunters

    TEST_MSG("Test_1: Testing getSafeRegion", dracula_getSafeRegion(dv) == REGION_1);
    printf("It should return MILAN but it returns %d\n", dracula_getMoveTowardsRegion(dv, REGION_1));
    TEST_MSG("Test_1: Testing getMoveTowardsRegion", dracula_getMoveTowardsRegion(dv, REGION_1) == MILAN);//, dv_get_location(dv, PLAYER_DRACULA)) == location_t);

    /////////////////////////////////////////////////////////
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

