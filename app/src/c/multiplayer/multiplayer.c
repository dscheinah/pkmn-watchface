#include <pebble.h>
#include "multiplayer.h"
#include "../render/helper.h"
#include "../render/monster.h"
#include "../render/window.h"

#define MP_START 0
#define MP_LOST  100
#define MP_WON   101

static Window* window;

static Layer* root;
static BitmapLayer* templateLayer;

static AppTimer* timer;

static DarkValue dark;

static MonsterPart allyPart = {.previous = 0};
static MonsterPart enemyPart = {.previous = 0};

static uint16_t allyHealth;
static uint16_t enemyHealth;

static void renderAllyHealth(Layer* layer, GContext* ctx) {
  helper_render_percentage_rect(layer, ctx, ALIGN_LEFT, helper_color_from_health(allyHealth), allyHealth);
}

static void renderEnemyHealth(Layer* layer, GContext* ctx) {
  helper_render_percentage_rect(layer, ctx, ALIGN_LEFT, helper_color_from_health(enemyHealth), enemyHealth);
}

static void window_load(Window* window) {
  root = window_create_center_layer(window);
  templateLayer = monster_create_template(root);
  monster_load_ally(root, &allyPart, dark);
  monster_load_enemy(root, &enemyPart, dark);
  layer_set_update_proc(allyPart.health, renderAllyHealth);
  layer_set_update_proc(enemyPart.health, renderEnemyHealth);
}

static void window_unload(Window* window) {
  monster_unload(&allyPart);
  monster_unload(&enemyPart);
  bitmap_layer_destroy(templateLayer);
  layer_destroy(root);
}

static void end() {
  window_stack_remove(window, true);
}

static void start() {
  window_stack_push(window, true);
  DictionaryIterator* outbox;
  app_message_outbox_begin(&outbox);
  if (outbox) {
    dict_write_cstring(outbox, MESSAGE_KEY_mp_steps,  "");
    dict_write_cstring(outbox, MESSAGE_KEY_mp_sleep,  "");
    dict_write_cstring(outbox, MESSAGE_KEY_mp_restful,  "");
    dict_write_cstring(outbox, MESSAGE_KEY_mp_active, "");
    if (app_message_outbox_send() != APP_MSG_OK) {
      end();
    }
  }
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
  }
  tuple = dict_find(iter, MESSAGE_KEY_mp_enemy_health);
  if (tuple) {
    enemyHealth = tuple->value->uint16;
  }
}

void multiplayer_init(State* state) {
  dark = state->settings & SETTINGS_DARK ? PBL_IF_BW_ELSE(DARK_FULL, DARK_ON) : DARK_OFF;
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
  switch (tuple->value->uint8) {
    case MP_START:
      start();
      break;
    case MP_LOST:
    case MP_WON:
      end();
      break;
  }
  update(iter);
  if (timer) {
    app_timer_cancel(timer);
  }
  timer = app_timer_register(60000, end, NULL);
}

void multiplayer_deinit() {
  window_destroy(window);
}
