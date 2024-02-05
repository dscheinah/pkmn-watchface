#pragma once
#include <pebble.h>
#include "../state/global.h"

void game_level(State* state);

bool game_damage(State* state);

void game_init(State* state);

void game_tick(State* state, bool reset);
