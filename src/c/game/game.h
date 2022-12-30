#pragma once
#include <pebble.h>
#include "../state/global.h"

void game_level_set_ally(State* state);

void game_level_set_enemy(State* state);

bool game_damage(State* state);

void game_init(State* state);

bool game_tick(State* state);

void game_reset(State* state);
