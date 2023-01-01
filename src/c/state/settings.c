#include <pebble.h>
#include "settings.h"

static void updateTimeFormat(State* state) {
  if (clock_is_24h_style()) {
    state->settings |= SETTINGS_TIME_FORMAT;
  } else {
    state->settings &= ~SETTINGS_TIME_FORMAT;
  }
}

void settings_init(State* state) {
  updateTimeFormat(state);
}

void settings_set(State* state, SettingsValue settings) {
  state->settings = settings;
  updateTimeFormat(state);
}

void settings_quiet_changed(State* state) {
  if (quiet_time_is_active()) {
    switch (state->quiet) {
      case QUIET_ON:
        break;
      case QUIET_TOGGLE_ON:
        state->quiet = QUIET_ON;
        break;
      default:
        state->quiet = QUIET_TOGGLE_ON;
        break;
    }
  } else {
    switch (state->quiet) {
      case QUIET_OFF:
        break;
      case QUIET_TOGGLE_OFF:
        state->quiet = QUIET_OFF;
        break;
      default:
        state->quiet = QUIET_TOGGLE_OFF;
        break;
    }
  }
}
