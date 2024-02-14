#pragma once
#include <pebble.h>
#include "../state/global.h"

Window* window_create_custom(State* state, WindowHandlers handlers);

Layer* window_create_center_layer(Window* window);
