#include <pebble.h>
#include "settings.h"
#define VERSION 1
#define VERSION_KEY 500
#define FLAGS_KEY 501
#define QUIET_KEY 502

static uint current = SETTINGS_DOW | SETTINGS_BLUETOOTH;
static int quietState = SETTINGS_QUIET_NONE;

static void updateTimeFormat() {
  if (clock_is_24h_style()) {
    current |= SETTINGS_TIME_FORMAT;
  } else {
    current &= ~SETTINGS_TIME_FORMAT;
  }
}

void settings_init() {
  if (persist_exists(VERSION_KEY) && persist_read_int(VERSION_KEY) == VERSION) {
    current = persist_read_int(FLAGS_KEY);
    quietState = persist_read_int(QUIET_KEY);
  }
  updateTimeFormat();
}

void settings_set(uint flags) {
  current = flags;
  updateTimeFormat();
}

uint settings_get() {
  return current;
}

int settings_quiet_changed() {
  if (quiet_time_is_active()) {
    if (quietState != SETTINGS_QUIET_ON) {
      quietState = SETTINGS_QUIET_ON;
      return SETTINGS_QUIET_ON;
    }
  } else {
    if (quietState != SETTINGS_QUIET_OFF) {
      quietState = SETTINGS_QUIET_OFF;
      return SETTINGS_QUIET_OFF;
    }
  }
  return SETTINGS_QUIET_NONE;
}

void settings_deinit() {
  persist_write_int(VERSION_KEY, VERSION);
  persist_write_int(FLAGS_KEY, current);
  persist_write_int(QUIET_KEY, quietState);
}
