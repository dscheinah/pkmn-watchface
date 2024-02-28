#include "multiplayer.h"
#include "../render/battlefield.h"
#include "../render/helper.h"
#include "../render/monster.h"
#include "../render/window.h"

#define MP_START 0
#define MP_FIGHT 10
#define MP_NEW   11
#define MP_LOST  100
#define MP_WON   101

#define VIBE_NONE   0
#define VIBE_SHORT  1
#define VIBE_LONG   2
#define VIBE_DOUBLE 3

static Window* window;

static Layer* root;
static BitmapLayer* templateLayer;
static TextLayer* textLayer;
static TextLayer* healthLayer;

static AppTimer* timer;

static State* state;
static DarkValue dark;

static MonsterPart allyPart = {.previous = 0};
static MonsterPart enemyPart = {.previous = 0};

static uint16_t allyHealth;
static uint16_t enemyHealth;

static char allyLevelBuffer[5];
static char enemyLevelBuffer[5];
static char healthBuffer[4];

static void end() {
  window_stack_remove(window, true);
}

static void event(char* text, int endTimeout, int vibeType) {
  text_layer_set_text(textLayer, text);
  if (timer) {
    app_timer_cancel(timer);
  }
  timer = app_timer_register(endTimeout, end, NULL);
  switch (vibeType) {
    case VIBE_SHORT:
      vibes_short_pulse();
      break;
    case VIBE_LONG:
      vibes_long_pulse();
      break;
    case VIBE_DOUBLE:
      vibes_double_pulse();
      break;
  }
  light_enable_interaction();
}

static bool start() {
  window_stack_push(window, true);
  DictionaryIterator* outbox;
  app_message_outbox_begin(&outbox);
  if (!outbox) {
    return false;
  }
  time_t reference = time_start_of_today() - (6 * SECONDS_PER_DAY);
  uint steps[6];
  uint sleep[6];
  uint restful[6];
  uint active[6];
  for (int i = 0; i < 6; i++) {
    steps[i] = PBL_IF_HEALTH_ELSE(
        health_service_sum(HealthMetricStepCount, reference, reference + SECONDS_PER_DAY) / 300,
        rand() % 100 + 1
    );
    sleep[i] = health_service_sum(HealthMetricSleepSeconds, reference, reference + SECONDS_PER_DAY) / SECONDS_PER_HOUR;
    restful[i] = health_service_sum(HealthMetricSleepRestfulSeconds, reference, reference + SECONDS_PER_DAY) * 5 / SECONDS_PER_HOUR;
    active[i] = health_service_sum(HealthMetricActiveSeconds, reference, reference + SECONDS_PER_DAY) / SECONDS_PER_MINUTE / 15;
    reference += SECONDS_PER_DAY;
  }
  char buffer[30];
  snprintf(buffer, 30, "%i,%i,%i,%i,%i,%i", steps[0], steps[1], steps[2], steps[3], steps[4], steps[5]);
  dict_write_cstring(outbox, MESSAGE_KEY_mp_steps, buffer);
  snprintf(buffer, 30, "%i,%i,%i,%i,%i,%i", sleep[0], sleep[1], sleep[2], sleep[3], sleep[4], sleep[5]);
  dict_write_cstring(outbox, MESSAGE_KEY_mp_sleep, buffer);
  snprintf(buffer, 30, "%i,%i,%i,%i,%i,%i", restful[0], restful[1], restful[2], restful[3], restful[4], restful[5]);
  dict_write_cstring(outbox, MESSAGE_KEY_mp_restful, buffer);
  snprintf(buffer, 30, "%i,%i,%i,%i,%i,%i", active[0], active[1], active[2], active[3], active[4], active[5]);
  dict_write_cstring(outbox, MESSAGE_KEY_mp_active, buffer);
  return app_message_outbox_send() == APP_MSG_OK;
}

static void update(DictionaryIterator* iter) {
  Tuple* tuple;

  tuple = dict_find(iter, MESSAGE_KEY_mp_ally);
  if (tuple) {
    monster_render(&allyPart, tuple->value->uint32, dark);
  }
  tuple = dict_find(iter, MESSAGE_KEY_mp_enemy);
  if (tuple) {
    monster_render(&enemyPart, tuple->value->uint32, dark);
  }
  tuple = dict_find(iter, MESSAGE_KEY_mp_ally_health);
  if (tuple) {
    allyHealth = tuple->value->uint16;
    snprintf(healthBuffer, 4, "%d", allyHealth);
    text_layer_set_text(healthLayer, healthBuffer);
  }
  tuple = dict_find(iter, MESSAGE_KEY_mp_enemy_health);
  if (tuple) {
    enemyHealth = tuple->value->uint16;
  }
  tuple = dict_find(iter, MESSAGE_KEY_mp_ally_level);
  if (tuple) {
    snprintf(allyLevelBuffer, 5, "L%d", tuple->value->uint16);
    text_layer_set_text(allyPart.level, allyLevelBuffer);
  }
  tuple = dict_find(iter, MESSAGE_KEY_mp_enemy_level);
  if (tuple) {
    snprintf(enemyLevelBuffer, 5, "L%d", tuple->value->uint16);
    text_layer_set_text(enemyPart.level, enemyLevelBuffer);
  }
}

static void renderAllyHealth(Layer* layer, GContext* ctx) {
  helper_render_percentage_rect(layer, ctx, ALIGN_LEFT, helper_color_from_health(allyHealth), allyHealth);
}

static void renderEnemyHealth(Layer* layer, GContext* ctx) {
  helper_render_percentage_rect(layer, ctx, ALIGN_LEFT, helper_color_from_health(enemyHealth), enemyHealth);
}

static void window_load(Window* window) {
  root = window_create_center_layer(window);
  templateLayer = monster_create_template(root);
  textLayer = helper_create_text_layer(root, GRect(9, 130, 129, 14), FONT_SMALL_BOLD, GTextAlignmentCenter, dark);
  monster_load_ally(root, &allyPart, dark);
  monster_load_enemy(root, &enemyPart, dark);
  layer_set_update_proc(allyPart.health, renderAllyHealth);
  layer_set_update_proc(enemyPart.health, renderEnemyHealth);
  healthLayer = helper_create_text_layer(root, GRect(73, 95, 59, 14), FONT_SMALL, GTextAlignmentRight, dark);
  text_layer_set_background_color(healthLayer, GColorClear);
}

static void window_unload(Window* window) {
  monster_unload(&allyPart);
  monster_unload(&enemyPart);
  text_layer_destroy(textLayer);
  bitmap_layer_destroy(templateLayer);
  layer_destroy(root);
  allyHealth = 0;
  enemyHealth = 0;
  battlefield_mark_dirty();
}

void multiplayer_init(State* stateRef) {
  state = stateRef;
  dark = state->settings & SETTINGS_DARK ? DARK_ON : DARK_OFF;
  window = window_create_custom(state, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
}

void multiplayer_handle_inbox(DictionaryIterator* iter) {
  Tuple* tuple = dict_find(iter, MESSAGE_KEY_mp_cmd);
  if (!tuple) {
    return;
  }
  if (tuple->value->uint8 == MP_START) {
    if (start()) {
      event("WAITING", 60000, VIBE_SHORT);
    } else {
      event("ERROR", 15000, VIBE_NONE);
    }
    return;
  }
  if (!window_stack_contains_window(window)) {
    return;
  }
  switch (tuple->value->uint8) {
    case MP_FIGHT:
      event("", 30000, VIBE_DOUBLE);
      break;
    case MP_NEW:
      event("", 30000, VIBE_SHORT);
      break;
    case MP_LOST:
      event("LOST", 15000, VIBE_LONG);
      break;
    case MP_WON:
      event("WON", 15000, VIBE_LONG);
      state->ally->type = allyPart.previous;
      break;
    default:
      event("ERROR", 15000, VIBE_NONE);
      break;
  }
  update(iter);
}

void multiplayer_deinit() {
  window_destroy(window);
}
