#include <pebble.h>
#include "settings.h"

static void updateSetting(State* state, SettingsValue setting, bool enabled) {
  if (enabled) {
    state->settings |= setting;
  } else {
    state->settings &= ~setting;
  }
}

void settings_init(State* state) {
  updateSetting(state, SETTINGS_TIME_FORMAT, clock_is_24h_style());
}

void settings_set(State* state, DictionaryIterator* iter) {
  int keys[] = {
    MESSAGE_KEY_date_format,
    MESSAGE_KEY_seconds,
    MESSAGE_KEY_dow,
    MESSAGE_KEY_bluetooth,
    MESSAGE_KEY_taps,
    MESSAGE_KEY_cache,
    MESSAGE_KEY_vibes,
  };
  SettingsValue settings[] = {
    SETTINGS_DATE_FORMAT,
    SETTINGS_SECONDS,
    SETTINGS_DOW,
    SETTINGS_BLUETOOTH,
    SETTINGS_TAPS,
    SETTINGS_CACHE,
    SETTINGS_VIBES,
  };
  for (int i = 0; i < 6; i++) {
    Tuple* tuple = dict_find(iter, keys[i]);
    if (tuple) {
      updateSetting(state, settings[i], (bool) tuple->value->int8);
    }
  }
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
