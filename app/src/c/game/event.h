#pragma once
#include <pebble.h>
#include "../state/global.h"

void event_next(State* state, int identifier);

int event_check();

bool event_boss();
