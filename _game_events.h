//
// Created by Andrew on 12/01/2019.
//

#ifndef ASS02_GAME_EVENTS_H
#define ASS02_GAME_EVENTS_H

#include "game.h"
#include "game_view.h"

void event_remove_vamp(GameView gv);
void event_remove_trap(GameView gv, location_t location);
bool event_player_hurt(GameView gv, enum player player, int damage);
static bool playerRested(GameView gv, enum player player);
void event_player_heal(GameView gv, enum player player, int amount);
bool event_encounter_trap(GameView gv, enum player player, location_t location);
void event_encounter_vamp(GameView gv);
bool event_encounter_dracula(GameView gv, enum player player);

#endif //ASS02_GAME_EVENTS_H
