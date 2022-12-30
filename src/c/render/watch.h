#pragma once
#include <pebble.h>
#include "../state/global.h"

void watch_load(Layer* root, Layer* cachedRoot, State* stateRef);

void watch_update_settings();

void watch_render_time(struct tm* tick_time);

void watch_render_date(struct tm* tick_time);

void watch_render_seconds(struct tm* tick_time);

void watch_unload();
