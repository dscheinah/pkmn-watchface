#include <pebble.h>
#include "ally.h"
#include "cache.h"
#include "const.h"
#include "battlefield.h"
#include "enemy.h"
#include "event.h"
#include "game.h"
#include "health.h"
#include "helper.h"
#include "watch.h"
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

static void gameTick(bool loop, bool reset, int identifier) {
  Health health = health_get_collected(loop, reset);
  game_set_ally_level(ally, health);
  game_set_enemy_level(enemy, health);
  if (loop) {
    if (reset) {
      ally_reset(ENEMY_COUNT - enemy->index_count + 10);
    }
    if (game_deal_damage(ally, enemy, health) && enemy_reset(*event)) {
      ally->level_modifier += 2;
    } else {
      if (enemy_evolution(*event)) {
        ally->level_modifier++;
      } else if (reset && (enemy_night() || enemy_hatch(health))) {
        ally->level_modifier++;
      }
    }
  } else if (enemy_quiet(quiet_time_is_active(), health)) {
    ally->level_modifier++;
  }
  event_next(enemy, health, identifier);
  ally_evolution();
  battlefield_mark_dirty();
}

static void handleTime(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & MINUTE_UNIT) {
    watch_render_time(tick_time);
  }
  if (units_changed & SECOND_UNIT) {
    watch_render_seconds(tick_time);
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
  if (charge_state.is_charging && enemy_charge()) {
    ally->level_modifier++;
  }
  battlefield_mark_dirty();
}

static void handleConnection(bool connected) {
  battlefield_set_enemy_missing(!connected);
}

static void handleInbox(DictionaryIterator *iter, void *context) {
  int flags = 0;
  Tuple *tuple;
  tuple = dict_find(iter, MESSAGE_KEY_date_format);
  if (tuple && (bool) tuple->value->int8) {
    flags |= WATCH_DATE_FORMAT;
  }
  tuple = dict_find(iter, MESSAGE_KEY_seconds);
  if (tuple && (bool) tuple->value->int8) {
    flags |= WATCH_SECONDS;
  }
  tuple = dict_find(iter, MESSAGE_KEY_dow);
  if (tuple && (bool) tuple->value->int8) {
    flags |= WATCH_DOW;
  }
  tuple = dict_find(iter, MESSAGE_KEY_bluetooth);
  if (tuple && (bool) tuple->value->int8) {
    flags |= WATCH_BLUETOOTH;
  }
  watch_set_settings(flags);
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
  watch_load(watchLayer, battlefieldLayer);
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
  watch_init();
  ally = ally_init();
  enemy = enemy_init();
  event = event_init();
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
    tick_timer_service_subscribe(watch_has_seconds() ? SECOND_UNIT : MINUTE_UNIT, handleTime);
  #endif
  time_t now = time(NULL);
  handleTime(localtime(&now), SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT | DAY_UNIT | INIT_UNIT);

  battery_state_service_subscribe(handleBattery);
  handleBattery(battery_state_service_peek());

  if (watch_has_bluetooth()) {
    connection_service_subscribe((ConnectionHandlers) {
      .pebble_app_connection_handler = handleConnection,
    });
    handleConnection(connection_service_peek_pebble_app_connection());
  }

  app_message_register_inbox_received(handleInbox);
  app_message_open(dict_calc_buffer_size(4, 4), 0);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();

  window_destroy(s_window);

  watch_deinit();
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
