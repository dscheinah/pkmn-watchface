#pragma once
#include <pebble.h>

void watch_load(Layer *root, Layer *cachedRoot, int settings);

void watch_set_settings(int settings);

void watch_render_time(struct tm *tick_time);

void watch_render_date(struct tm *tick_time);

void watch_render_seconds(struct tm *tick_time, int settings);

void watch_unload();
