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

void settings_set(State* state, DictionaryIterator* iter) {
  Tuple* tuple;
  state->settings = 0;
  tuple = dict_find(iter, MESSAGE_KEY_date_format);
  if (tuple && (bool) tuple->value->int8) {
    state->settings |= SETTINGS_DATE_FORMAT;
  }
  tuple = dict_find(iter, MESSAGE_KEY_seconds);
  if (tuple && (bool) tuple->value->int8) {
    state->settings |= SETTINGS_SECONDS;
  }
  tuple = dict_find(iter, MESSAGE_KEY_dow);
  if (tuple && (bool) tuple->value->int8) {
    state->settings |= SETTINGS_DOW;
  }
  tuple = dict_find(iter, MESSAGE_KEY_bluetooth);
  if (tuple && (bool) tuple->value->int8) {
    state->settings |= SETTINGS_BLUETOOTH;
  }
  tuple = dict_find(iter, MESSAGE_KEY_taps);
  if (tuple && (bool) tuple->value->int8) {
    state->settings |= SETTINGS_TAPS;
  }
  updateTimeFormat(state);
}

#if !defined(TEST)
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
#endif
