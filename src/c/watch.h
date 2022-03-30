#pragma once
#include <pebble.h>
#define WATCH_TIME_FORMAT 1
#define WATCH_DATE_FORMAT 2
#define WATCH_SECONDS 8
#define WATCH_DOW 16
#define WATCH_WEEK_NUMBER 32
#define WATCH_YEAR 64
#define WATCH_BLUETOOTH 128

void watch_init();

void watch_load(Layer *root);

void watch_render_time(struct tm *tick_time);

void watch_render_date(struct tm *tick_time);

void watch_set_settings(int flags, Layer *root);

bool watch_has_seconds();

bool watch_has_bluetooth();

void watch_unload();

void watch_deinit();
