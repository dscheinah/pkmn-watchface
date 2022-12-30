#pragma once
#include <pebble.h>
#include "../state/global.h"

void helper_evolve(State* state, int type, int level, bool reset);

int helper_bird(State* state);
