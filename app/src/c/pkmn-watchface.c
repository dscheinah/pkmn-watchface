#include <pebble.h>
#include "state/global.h"
#include "state/state.h"
#include "state/settings.h"
#include "render/layout.h"
#include "render/battlefield.h"
#include "render/monster.h"
#include "render/watch.h"
#include "render/window.h"
#include "ally/ally.h"
#include "enemy/enemy.h"
#include "game/event.h"
#include "game/game.h"
#include "health/health.h"
#include "multiplayer/multiplayer.h"
#if defined(TEST)
  #include "test.h"
#endif

#define INIT_UNIT 128

static Window* window;

static State* state;

static const uint32_t durations[] = {150, 400, 50, 100, 100, 100, 300};
static const VibePattern vibes = {
  .durations = durations,
  .num_segments = ARRAY_LENGTH(durations),
};

bool tapActive = false;

static void sendPokedex() {
  DictionaryIterator* outbox;
  app_message_outbox_begin(&outbox);
  if (outbox) {
    dict_write_int(outbox, MESSAGE_KEY_pokedex0, &state->index[0], 4, false);
    dict_write_int(outbox, MESSAGE_KEY_pokedex1, &state->index[1], 4, false);
    app_message_outbox_send();
  }
}

static void markDirty() {
  if (state_update_index()) {
    state_write();
    if ((state->settings & SETTINGS_VIBES) && !quiet_time_is_active()) {
      vibes_enqueue_custom_pattern(vibes);
    }
  }
  battlefield_mark_dirty();
}

static void game(struct tm* tick_time, TimeUnits units_changed) {
  #if defined(TEST)
    if (time(NULL) % 3 == 0) {
      units_changed |= HOUR_UNIT;
    }
  #endif
  if (!(units_changed & HOUR_UNIT)) {
    return;
  }
  bool aprilsFool = tick_time->tm_mon == 3 && tick_time->tm_mday == 1;
  if (aprilsFool) {
    state->ally->type = RESOURCE_ID_a142;
  }
  bool reset = (units_changed & DAY_UNIT) && !(units_changed & INIT_UNIT);
  #if defined(TEST)
    reset = test_day();
  #endif
  settings_quiet_changed(state);
  health_refresh(state->health, tick_time->tm_hour, reset);
  game_init(state);
  if (!(units_changed & INIT_UNIT)) {
    health_update(state->health, reset);
    game_tick(state, reset);
    if (aprilsFool) {
      state->ally->type = RESOURCE_ID_a142;
    }
  }
  #if defined(TEST)
    test_next_tick(state);
  #endif
  event_next(state, tick_time->tm_hour);
  markDirty();
}

static void handleTime(struct tm* tick_time, TimeUnits units_changed) {
  if (units_changed & SECOND_UNIT) {
    watch_render_seconds(tick_time);
  }
  if (units_changed & MINUTE_UNIT) {
    if (tapActive) {
      tapActive = false;
      tick_timer_service_subscribe(MINUTE_UNIT, handleTime);
    }
    watch_render_time(tick_time);
  }
  if (units_changed & DAY_UNIT) {
    watch_render_date(tick_time);
  }
  game(tick_time, units_changed);
}

static void handleBattery(BatteryChargeState charge_state) {
  state->charging = charge_state.is_charging;
  state->ally->health = charge_state.charge_percent;
  if (charge_state.is_charging && enemy_charge(state)) {
    state->ally->level_modifier++;
  }
  markDirty();
}

static void handleConnection(bool connected) {
  bool missing = !connected;
  if (state->missing ^ missing) {
    state->missing = missing;
    markDirty();
  }
}

static void handleTap(AccelAxisType axis, int32_t direction) {
  if (axis != ACCEL_AXIS_Y) {
    tapActive = true;
    tick_timer_service_subscribe(SECOND_UNIT, handleTime);
    time_t now = time(NULL);
    handleTime(localtime(&now), SECOND_UNIT);
    light_enable_interaction();
  }
}

static void handleInbox(DictionaryIterator* iter, void* context) {
  settings_set(state, iter);
  multiplayer_handle_inbox(iter);
  Tuple* tuple = dict_find(iter, MESSAGE_KEY_ally);
  if (tuple) {
    ally_switch(state->ally, tuple->value->uint8 - 48);
    ally_evolution(state->ally);
    battlefield_mark_dirty();
  }
  state_write();
  sendPokedex();
}

static void prv_window_load(Window* window) {
  layout_load(window, state);
  battlefield_load(layout_get_battlefield(), state);
  watch_load(layout_get_watch(), state);
}

static void prv_window_unload(Window* window) {
  watch_unload();
  battlefield_unload();
  layout_unload();
}

static void prv_init(void) {
  state = state_init();
  multiplayer_init(state);
  monster_init(state);
  settings_init(state);

  window = window_create_custom(state, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  window_stack_push(window, false);

  #if defined(TEST)
    tick_timer_service_subscribe(SECOND_UNIT, handleTime);
  #else
    tick_timer_service_subscribe(state->settings & SETTINGS_SECONDS ? SECOND_UNIT : MINUTE_UNIT, handleTime);
  #endif
  time_t now = time(NULL);
  handleTime(localtime(&now), SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT | DAY_UNIT | INIT_UNIT);

  battery_state_service_subscribe(handleBattery);
  handleBattery(battery_state_service_peek());

  if (state->settings & SETTINGS_BLUETOOTH) {
    connection_service_subscribe((ConnectionHandlers) {
      .pebble_app_connection_handler = handleConnection,
    });
    handleConnection(connection_service_peek_pebble_app_connection());
  } else {
    state->missing = false;
  }

  if (state->settings & SETTINGS_TAPS) {
    tapActive = true;
    accel_tap_service_subscribe(handleTap);
  }

  app_message_register_inbox_received(handleInbox);
  app_message_open(128, 128);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();
  accel_tap_service_unsubscribe();

  window_destroy(window);

  state_write();

  app_message_deregister_callbacks();

  monster_deinit();
  multiplayer_deinit();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
