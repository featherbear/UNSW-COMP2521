//
// Created by Andrew on 12/01/2019.
//

#include "_game_events.h"


void event_remove_vamp(GameView gv) {
    gv->timers.vampFlyTime = 0;
    gv->encounters.vamp_location = NOWHERE;
}

void event_remove_trap(GameView gv, location_t location) {
    assert(gv->encounters.traps[location] > 0);

    gv->encounters.traps[location]--;
}

/*
 * Player Hurt Event
 * Reduces player health by `damage`.
 * If the player is a hunter and their health drops to zero, send them to the hospital and decrease the game score
 */
bool event_player_hurt(GameView gv, enum player player, int damage) {

    bool result = true;

    // Pointer to the player's health
    // ... a POINTER because I'm lazy to keep referencing the entire gv->players[player].health each time
    // Ermm... I didn't mean that kinda comment.. but yay for laziness
    int *health = &gv->players[player].health;

    *health -= damage;
    printf_red("> Player %d took %d damage! (HP: %d)\n", player, damage, *health);

    if (player != PLAYER_DRACULA) {
        if (*health <= 0) {
            result = false;
            gv->score -= SCORE_LOSS_HUNTER_HOSPITAL;
            printf_yellow("> Player %d dun guf. he ded. rip m9\n", player);

//            dlist_push(gv->players[player].moves, HOSPITAL_LOCATION)

            // TODO ?? while in the hospital, they have zero life points

            // TODO Move to hospital -------
            // what happens to the trail just adds hospital to trail I'm preee sure..

        }
    }

    return result;
}

/*
 * Player Heal Event
 * Heals a given player - Hunters are capped at 9 health points
 */
void event_player_heal(GameView gv, enum player player, int amount) {


    int *health = &gv->players[player].health;
    *health += amount;
    printf_green("Player %d recovered %d health points (HP: %d)\n", player, amount, *health);
    if (*health > GAME_START_HUNTER_LIFE_POINTS) printf_green("Health capped at 9 HP\n");
    // Cap health for Hunters
    if (player != PLAYER_DRACULA) *health = min(GAME_START_HUNTER_LIFE_POINTS, *health);
}


/*
 * Trap Encounter Event
 * Damage the player by 2 health points, then remove trap
 */
bool event_encounter_trap(GameView gv, enum player player, location_t location) {

    assert(player != PLAYER_DRACULA);
    assert(gv->encounters.traps[location] > 0);

    printf_yellow("> Player %d encountered a trap!\n", player);

    event_remove_trap(gv, location);
    return event_player_hurt(gv, player, LIFE_LOSS_TRAP_ENCOUNTER);
}

/*
 * Vampire Encounter Event
 * Reduce the game score by 13, then remove vampire
 */
void event_encounter_vamp(GameView gv) {
    printf_red("> A vampire has matured. players dun guf again smh fake hunterzz!\n");

    gv->score -= SCORE_LOSS_VAMPIRE_MATURES;
    event_remove_vamp(gv);
}

/*
 * Dracula Encounter Event
 * Damage the player by 4, and Dracula by 10
 */
bool event_encounter_dracula(GameView gv, enum player player) {
    assert(player != PLAYER_DRACULA);

    printf_yellow("> BAM! Player %d encountered Dracula at %s!\n", player, location_get_name(gv_get_location(gv, player)));

    event_player_hurt(gv, PLAYER_DRACULA, LIFE_LOSS_HUNTER_ENCOUNTER);
    return event_player_hurt(gv, player, LIFE_LOSS_DRACULA_ENCOUNTER);
}

