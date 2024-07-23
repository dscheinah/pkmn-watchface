#pragma once
#include <pebble.h>
#include "../state/global.h"

void helper_evolve(State* state, ResourceValue type, int level, bool reset);

ResourceValue helper_bird(State* state);

ResourceValue helper_morph(State* state, bool morph);
