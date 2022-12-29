#pragma once
#define SETTINGS_BLUETOOTH 1
#define SETTINGS_TIME_FORMAT 2
#define SETTINGS_DATE_FORMAT 4
#define SETTINGS_SECONDS 8
#define SETTINGS_DOW 16
#define SETTINGS_TAPS 32
#define SETTINGS_QUIET_OFF -1
#define SETTINGS_QUIET_NONE 0
#define SETTINGS_QUIET_ON 1

void settings_init();

void settings_set(uint flags);

uint settings_get();

int settings_quiet_changed();

void settings_deinit();
