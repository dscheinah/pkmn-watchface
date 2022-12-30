#include <pebble.h>
#include "settings.h"

static State* state;

static void updateTimeFormat() {
  if (clock_is_24h_style()) {
    state->settings |= SETTINGS_TIME_FORMAT;
  } else {
    state->settings &= ~SETTINGS_TIME_FORMAT;
  }
}

void settings_init(State* stateRef) {
  state = stateRef;
  updateTimeFormat();
}

void settings_set(SettingsValue settings) {
  state->settings = settings;
  updateTimeFormat();
}

void settings_quiet_changed() {
  if (quiet_time_is_active()) {
    if (state->quiet != QUIET_ON) {
      state->quiet = QUIET_ON;
      return;
    }
  } else {
    if (state->quiet != QUIET_OFF) {
      state->quiet = QUIET_OFF;
      return;
    }
  }
  state->quiet = QUIET_NONE;
}
