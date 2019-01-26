////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include "game.h"
// #include "dracula.h" ??????
#include "dracula_view.h"
#include "_queue.h"
#include "_ai.h"

#include <stdlib.h>

// #include "_structures.h"


#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#endif


/* Main Function */
void decide_dracula_move(DraculaView dv)
{
    location_t get_dracula_move(dv);
    // char *msg = get_dracula_taunt();
    register_best_play(location_get_abbrev(l), "HAH! Gotchu");
}


// Get possible dracula moves which don't overlap with possible hunter moves
Queue dracula_getSafeMoves(DraculaView dv)
{
    Queue hunter_plays = queue_new();
    Queue dracula_plays = queue_new();
    Queue tmp_q;
    location_t *tmp_a;
    size_t size;

    // Get all the hunter plays
    for (int i = 0; i < NUM_PLAYERS - 1; i++) {
        tmp_a = dv_get_dests_player(dv, &size, i, true, true, true):
        tmp_q = convert_aTOq(tmp_a, size);
        printf("Queue size for hunter moves is now: %d\n", queue_size(tmp_q));
        queue_append(hunter_plays, tmp_q);
    }

    // Get all the dracula plays
    tmp_a = dv_get_dests(dv, &size, true true);
    tmp_q = convert_aTOq(tmp_a, size);

    // If dracula's dests are found in hunter's dests, delete them
    // Not sure if this is the most optimal way but.. This'll do for now
    for (size_t i = 0; i < size; i++) {
        int location = queue_de(tmp_q);
        if (!queue_has(hunter_plays, location)) queue_en(dracula_plays, location);
    }

    queue_drop(hunter_plays);
    free(tmp_a);
    queue_drop(tmp_q);
    return dracula_plays;
}

/* Find the shortest path to a location
 * Uses a minimum spanning tree */
location_t get_shortestPath(DraculaView dv, Map m, Queue dMoves, location_t target)
{
    puts("==> Getting the shortest path")
    bool hasSeen[MAX_NUM_LOCATIONS] = {false};
    location_t from[MAX_NUM_LOCATIONS] = {};
    for (int i = 0; i < MAX_NUM_LOCATIONS; i++) from[i] = -1;
    Queue q = queue_new();
        // Debugging purposes
        int src = queue_de(dMoves)
        printf("==>First Item in queue: %d\n", src);
        queue_en(src);
    //queue_en(queue_de(dMoves));

        while (queue_size(dMoves) != 0) {
            int l = queue_de(tmp);
            for (dNode node = m->connections[l]; node != NULL; node = node->next) {
                if (hasSeen[node->item] == false) {
                    // Debugging
                    printf("==>Checking location %d, haven't seen, got here from %d\n", node->item, l);
                    hasSeen[node->item] = true;
                    from[node->item] = l;
                    if (node->item == target) break;
                }
                // Debugging
                printf("==>Checking location %d, seen, got here from %d\n", node->item, from[node->item]);
            }
        }
        // Debuggins
        printf("==> Finished queue, found target.. printing path: (Backwards) {");
        int tmp = (int)target;
        while (from[tmp] != -1) { // <-- is that right.. we want the value before source
            printf("%d", tmp);
            tmp = from[tmp];
        }

        // tmp should be the value before -1
        return tmp;

}

/* Get the most optimum move */
location_t get_dracula_move(DraculaView dv)
{
    if (dv_get_round(dv) == 0) return = BR;

    Queue dMoves = dracula_getSafeMoves(dv);
        // Debugging purposes ONLY
        if (queue_size(dMoves) == 0) {  // Rip we dun goof...
            // Hopefully I can ensure that this doesn't happen..
            // I feel like this will happen once all the hunters are kinda close together.. but like.. what's a better waay to do this..
            size_t size;
            location_t l = dv_get_dests(dv, &size, true, true)
            free(l);

            return = l[0];
        }

    Map m = map_new();
    if (health < 15) return = get_shortestPath(dv, m, dMoves, CASTLE_DRACULA);
    else return queue_de(dMoves);

    return bestLocation;
}