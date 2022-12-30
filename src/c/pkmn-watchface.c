#include <pebble.h>
#include "ally/ally.h"
#include "state/global.h"
#include "state/state.h"
#include "render/layout.h"
#include "render/battlefield.h"
#include "enemy/enemy.h"
#include "game/event.h"
#include "game/game.h"
#include "health/health.h"
#include "render/watch.h"
#include "state/settings.h"
#if defined(TEST)
  #include "test.h"
#endif
#define INIT_UNIT 128

static Window* s_window;

static State* state;

bool tapActive = false;

static void gameTick(bool loop, bool reset, int identifier) {
  health_update(state->health, loop, reset);
  game_init(state);
  if (loop) {
    bool canReset = game_tick(state);
    if (reset) {
      ally_reset(state->ally, ENEMY_COUNT - state->enemy->index_count + 10);
      if (canReset) {
        game_reset(state);
      }
    }
  }
  event_next(state, identifier);
  ally_evolution(state->ally);
  battlefield_mark_dirty();
}

static void handleTime(struct tm* tick_time, TimeUnits units_changed) {
  if (units_changed & SECOND_UNIT) {
    watch_render_seconds(tick_time);
  }
  if (units_changed & MINUTE_UNIT) {
    #if !defined(TEST)
      if (tapActive) {
        tapActive = false;
        tick_timer_service_subscribe(MINUTE_UNIT, handleTime);
      }
    #endif
    watch_render_time(tick_time);
  }
  bool day = units_changed & DAY_UNIT, loop = !(units_changed & INIT_UNIT);
  if (day) {
    watch_render_date(tick_time);
  }
  #if defined(TEST)
    if (!loop || time(NULL) % 5 == 0) {
      test_health_refresh(state->health);
      gameTick(loop, test_day(), tick_time->tm_hour);
      test_next_tick(state);
    }
  #else
    if (units_changed & HOUR_UNIT) {
      health_refresh(state->health, day);
      gameTick(loop, day, tick_time->tm_hour);
    }
  #endif
}

static void handleBattery(BatteryChargeState charge_state) {
  state->ally->health = charge_state.charge_percent;
  if (charge_state.is_charging && enemy_charge(state)) {
    state->ally->level_modifier++;
  }
  battlefield_mark_dirty();
}

static void handleConnection(bool connected) {
  bool missing = !connected;
  if (state->missing ^ missing) {
    state->missing = missing;
    battlefield_mark_dirty();
  }
}

static void handleTap(AccelAxisType axis, int32_t direction) {
  if (axis == ACCEL_AXIS_Z) {
    tapActive = true;
    tick_timer_service_subscribe(SECOND_UNIT, handleTime);
  }
}

static void handleInbox(DictionaryIterator* iter, void* context) {
  SettingsValue flags = 0;
  Tuple* tuple;
  tuple = dict_find(iter, MESSAGE_KEY_date_format);
  if (tuple && (bool) tuple->value->int8) {
    flags |= SETTINGS_DATE_FORMAT;
  }
  tuple = dict_find(iter, MESSAGE_KEY_seconds);
  if (tuple && (bool) tuple->value->int8) {
    flags |= SETTINGS_SECONDS;
  }
  tuple = dict_find(iter, MESSAGE_KEY_dow);
  if (tuple && (bool) tuple->value->int8) {
    flags |= SETTINGS_DOW;
  }
  tuple = dict_find(iter, MESSAGE_KEY_bluetooth);
  if (tuple && (bool) tuple->value->int8) {
    flags |= SETTINGS_BLUETOOTH;
  }
  tuple = dict_find(iter, MESSAGE_KEY_taps);
  if (tuple && (bool) tuple->value->int8) {
    flags |= SETTINGS_TAPS;
  }
  settings_set(flags);
  watch_update_settings();
}

static void prv_window_load(Window* window) {
  layout_load(window_get_root_layer(window));
  battlefield_load(layout_get_battlefield(), state);
  watch_load(layout_get_watch(), layout_get_root(), state);
}

static void prv_window_unload(Window* window) {
  watch_unload();
  battlefield_unload();
  layout_unload();
}

static void prv_init(void) {
  state = state_init();
  settings_init(state);
  health_init();

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = false;
  window_stack_push(s_window, animated);

  #if defined(TEST)
    tick_timer_service_subscribe(SECOND_UNIT, handleTime);
  #else
    tick_timer_service_subscribe(state->settings & SETTINGS_SECONDS ? SECOND_UNIT : MINUTE_UNIT, handleTime);
  #endif
  time_t now = time(NULL);
  handleTime(localtime(&now), SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT | DAY_UNIT | INIT_UNIT);

  battery_state_service_subscribe(handleBattery);
  handleBattery(battery_state_service_peek());

  if (state->settings && SETTINGS_BLUETOOTH) {
    connection_service_subscribe((ConnectionHandlers) {
      .pebble_app_connection_handler = handleConnection,
    });
    handleConnection(connection_service_peek_pebble_app_connection());
  }

  if (state->settings & SETTINGS_TAPS) {
    tapActive = true;
    accel_tap_service_subscribe(handleTap);
  }

  app_message_register_inbox_received(handleInbox);
  app_message_open(dict_calc_buffer_size(5, 5), 0);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();
  accel_tap_service_unsubscribe();

  window_destroy(s_window);

  state_deinit();

  app_message_deregister_callbacks();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
