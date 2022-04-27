#pragma once
#include <pebble.h>
#define WATCH_BLUETOOTH 1
#define WATCH_TIME_FORMAT 2
#define WATCH_DATE_FORMAT 4
#define WATCH_SECONDS 8
#define WATCH_DOW 16

void watch_init();

void watch_load(Layer *root, Layer *cachedRoot);

void watch_render_time(struct tm *tick_time);

void watch_render_date(struct tm *tick_time);

void watch_render_seconds(struct tm *tick_time);

void watch_set_settings(uint flags);

bool watch_has_seconds();

bool watch_has_bluetooth();

void watch_unload();

void watch_deinit();
