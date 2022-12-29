#pragma once
#include <pebble.h>
#define WATCH_BLUETOOTH 1
#define WATCH_TIME_FORMAT 2
#define WATCH_DATE_FORMAT 4
#define WATCH_SECONDS 8
#define WATCH_DOW 16
#define WATCH_TAPS 32
#define WATCH_QUIET_OFF -1
#define WATCH_QUIET_NONE 0
#define WATCH_QUIET_ON 1

void watch_init();

void watch_load(Layer *root, Layer *cachedRoot);

void watch_render_time(struct tm *tick_time);

void watch_render_date(struct tm *tick_time);

void watch_render_seconds(struct tm *tick_time);

void watch_set_settings(uint flags);

bool watch_has_seconds();

bool watch_has_bluetooth();

bool watch_has_taps();

int watch_quiet_changed();

void watch_unload();

void watch_deinit();
