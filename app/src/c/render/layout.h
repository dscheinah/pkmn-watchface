#pragma once
#include <pebble.h>
#include "../state/global.h"

void layout_load(Window* window, State* state);

Layer* layout_get_battlefield();

Layer* layout_get_watch();

void layout_unload();
