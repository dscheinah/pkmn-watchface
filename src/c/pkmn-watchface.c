#include <pebble.h>
#include "ally/ally.h"
#include "render/cache.h"
#include "state/const.h"
#include "render/battlefield.h"
#include "enemy/enemy.h"
#include "game/event.h"
#include "game/game.h"
#include "health/health.h"
#include "render/helper.h"
#include "render/watch.h"
#include "state/settings.h"
#if defined(TEST)
  #include "test.h"
#endif
#define INIT_UNIT 128

static Window *s_window;

static Layer *watchLayer;
static BitmapLayer *templateLayer;
static GBitmap *templateBitmap;

static Ally *ally;
static Enemy *enemy;
static EventValue *event;

bool tapActive = false;

static void gameTick(bool loop, bool reset, int identifier) {
  Health health = health_get_collected(loop, reset);
  game_level_set_ally(ally, health);
  game_level_set_enemy(enemy, health);
  if (loop) {
    if (reset) {
      ally_reset(ally, ENEMY_COUNT - enemy->index_count + 10);
    }
    if (game_damage(ally, enemy, health) && (enemy_reset_bird(enemy, ally) || enemy_reset(enemy, *event))) {
      ally->level_modifier += 2;
    } else {
      if (enemy_evolution(enemy, *event)) {
        ally->level_modifier++;
      } else if (reset && (enemy_evolution_night(enemy) || enemy_hatch(enemy, health))) {
        ally->level_modifier++;
      } else if (enemy_hatch_bird(enemy, ally)) {
        ally->level_modifier++;
      }
    }
  } else {
    switch (settings_quiet_changed()) {
      case SETTINGS_QUIET_ON:
        if (enemy_quiet_enable(enemy, health)) {
          ally->level_modifier++;
        }
        break;
      case SETTINGS_QUIET_OFF:
        if (enemy_quiet_disable(enemy)) {
          ally->level_modifier++;
        }
        break;
    }
  }
  event_next(enemy, health, identifier);
  ally_evolution(ally);
  battlefield_mark_dirty();
}

static void handleTime(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & SECOND_UNIT) {
    watch_render_seconds(tick_time, settings_get());
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
      test_health_refresh();
      gameTick(loop, test_day(), tick_time->tm_hour);
      test_next_tick(enemy, event);
    }
  #else
    if (units_changed & HOUR_UNIT) {
      health_refresh(day);
      gameTick(loop, day, tick_time->tm_hour);
    }
  #endif
}

static void handleBattery(BatteryChargeState charge_state) {
  ally->health = charge_state.charge_percent;
  if (charge_state.is_charging && enemy_charge(enemy)) {
    ally->level_modifier++;
  }
  battlefield_mark_dirty();
}

static void handleConnection(bool connected) {
  bool missing = !connected;
  if (enemy->missing ^ missing) {
    enemy->missing = missing;
    battlefield_mark_dirty();
  }
}

static void handleTap(AccelAxisType axis, int32_t direction) {
  if (axis == ACCEL_AXIS_Z) {
    tapActive = true;
    tick_timer_service_subscribe(SECOND_UNIT, handleTime);
  }
}

static void handleInbox(DictionaryIterator *iter, void *context) {
  int flags = 0;
  Tuple *tuple;
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
  watch_set_settings(settings_get());
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int x = (bounds.size.w - 144) / 2;
  int y = (bounds.size.h - 168) / 2;
  GRect coords = GRect(x, y, 144, 168);

  templateLayer = bitmap_layer_create(coords);
  templateBitmap = gbitmap_create_with_resource(RESOURCE_ID_template);
  bitmap_layer_set_bitmap(templateLayer, templateBitmap);

  Layer *battlefieldLayer = bitmap_layer_get_layer(templateLayer);
  battlefield_load(battlefieldLayer, ally, enemy, event);
  cache_layer_create(window_layer, battlefieldLayer);

  watchLayer = layer_create(coords);
  layer_add_child(window_layer, watchLayer);
  watch_load(watchLayer, battlefieldLayer, settings_get());
}

static void prv_window_unload(Window *window) {
  watch_unload();
  battlefield_unload();
  cache_layer_destroy();
  gbitmap_destroy(templateBitmap);
  bitmap_layer_destroy(templateLayer);
  layer_destroy(watchLayer);
}

static void prv_init(void) {
  settings_init();
  ally = ally_init();
  enemy = enemy_init();
  event = event_init();
  health_init();

  uint settings = settings_get();

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
    tick_timer_service_subscribe(settings & SETTINGS_SECONDS ? SECOND_UNIT : MINUTE_UNIT, handleTime);
  #endif
  time_t now = time(NULL);
  handleTime(localtime(&now), SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT | DAY_UNIT | INIT_UNIT);

  battery_state_service_subscribe(handleBattery);
  handleBattery(battery_state_service_peek());

  if (settings && SETTINGS_BLUETOOTH) {
    connection_service_subscribe((ConnectionHandlers) {
      .pebble_app_connection_handler = handleConnection,
    });
    handleConnection(connection_service_peek_pebble_app_connection());
  }

  if (settings & SETTINGS_TAPS) {
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

  settings_deinit();
  ally_deinit();
  enemy_deinit();
  event_deinit();
  health_deinit();

  app_message_deregister_callbacks();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
