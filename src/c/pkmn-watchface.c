#include <pebble.h>
#include "ally.h"
#include "battlefield.h"
#include "enemy.h"
#include "game.h"
#include "health.h"
#include "helper.h"
#if defined(TEST)
  #include "test.h"
#endif
#define INIT_UNIT 128

static Window *s_window;

static BitmapLayer *templateLayer;
static TextLayer *timeLayer;
static TextLayer *dateLayer;

static GBitmap *templateBitmap;

static Ally *ally;
static Enemy *enemy;

static void gameTick(bool loop, bool reset, int event) {
  Health health = health_get_collected(loop, reset);
  game_set_ally_level(ally, health);
  game_set_enemy_level(enemy, health);
  if (loop) {
    if (reset) {
      ally_reset();
      if (enemy_night()) {
        ally->level_modifier++;
      }
    }
    if (enemy_evolution(health, event)) {
      ally->level_modifier++;
    }
    if (game_deal_damage(ally, enemy, health) && enemy_reset(event == 0, event == 1)) {
      ally->level_modifier += 2;
    }
    if (reset && enemy_hatch(health)) {
      ally->level_modifier++;
    }
  }
  ally_evolution();
  battlefield_mark_dirty();
}

static void renderTime(struct tm *tick_time) {
  static char timeBuffer[6];
  strftime(timeBuffer, 6, clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(timeLayer, timeBuffer);
}

static void renderDate(struct tm *tick_time) {
  static char dateBuffer[8];
  strftime(dateBuffer, 8, "%m / %d", tick_time);
  text_layer_set_text(dateLayer, dateBuffer);
}

static void handleTime(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & MINUTE_UNIT) {
    renderTime(tick_time);
  }
  bool day = units_changed & DAY_UNIT, loop = !(units_changed & INIT_UNIT);
  if (day) {
    renderDate(tick_time);
  }
  #if defined(TEST)
    if (!loop || time(NULL) % 5 == 0) {
      test_health_refresh();
      gameTick(loop, test_day(), test_event());
      test_next_tick(ally, enemy);
    }
  #else
    if (units_changed & HOUR_UNIT) {
      health_refresh(day);
      gameTick(loop, day, rand() % 5);
    }
  #endif
}

static void handleBattery(BatteryChargeState charge_state) {
  ally->health = charge_state.charge_percent;
  if (charge_state.is_charging) {
    if (enemy_charge()) {
      ally->level_modifier++;
    }
  }
  battlefield_mark_dirty();
}

static void handleConnection(bool connected) {
  battlefield_set_enemy_missing(!connected);
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int x = (bounds.size.w - 144) / 2;
  int y = (bounds.size.h - 168) / 2;

  templateLayer = bitmap_layer_create(GRect(x, y, 144, 168));
  timeLayer = helper_create_text_layer(GRect(5, 121, 139, 32), FONT_KEY_LECO_32_BOLD_NUMBERS, GTextAlignmentCenter);
  dateLayer = helper_create_text_layer(GRect(73, 96, 60, 14), FONT_KEY_GOTHIC_14, GTextAlignmentRight);

  Layer *centerLayer = bitmap_layer_get_layer(templateLayer);
  layer_add_child(window_layer, centerLayer);
  layer_add_child(centerLayer, text_layer_get_layer(timeLayer));
  layer_add_child(centerLayer, text_layer_get_layer(dateLayer));

  templateBitmap = gbitmap_create_with_resource(RESOURCE_ID_template);
  bitmap_layer_set_bitmap(templateLayer, templateBitmap);

  battlefield_load(centerLayer, ally, enemy);
}

static void prv_window_unload(Window *window) {
  battlefield_unload();

  gbitmap_destroy(templateBitmap);

  bitmap_layer_destroy(templateLayer);
  text_layer_destroy(timeLayer);
  text_layer_destroy(dateLayer);
}

static void prv_init(void) {
  ally = ally_init();
  enemy = enemy_init();
  health_init();

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = false;
  window_stack_push(s_window, animated);

  time_t now = time(NULL);
  #if defined(TEST)
    tick_timer_service_subscribe(SECOND_UNIT, handleTime);
  #else
    tick_timer_service_subscribe(MINUTE_UNIT, handleTime);
  #endif
  handleTime(localtime(&now), MINUTE_UNIT | HOUR_UNIT | DAY_UNIT | INIT_UNIT);

  battery_state_service_subscribe(handleBattery);
  handleBattery(battery_state_service_peek());

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = handleConnection,
  });
  handleConnection(connection_service_peek_pebble_app_connection());
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();

  window_destroy(s_window);

  ally_deinit();
  enemy_deinit();
  health_deinit();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
