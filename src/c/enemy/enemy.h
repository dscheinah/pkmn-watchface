#pragma once
#include <pebble.h>
#include "../state/global.h"

bool enemy_reset(State* state, bool teleport);

bool enemy_reset_bird(State* state);

bool enemy_evolution(State* state);

bool enemy_evolution_night(State* state);

bool enemy_hatch(State* state);

bool enemy_hatch_bird(State* state);

bool enemy_charge(State* state);

bool enemy_quiet_enable(State* state);

bool enemy_quiet_disable(State* state);
