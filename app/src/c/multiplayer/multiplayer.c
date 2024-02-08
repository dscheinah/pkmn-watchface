#include "multiplayer.h"
#include "../render/helper.h"
#include "../render/monster.h"
#include "../render/window.h"

#define MP_START 0
#define MP_FIGHT 10
#define MP_LOST  100
#define MP_WON   101

static Window* window;

static Layer* root;
static BitmapLayer* templateLayer;
static TextLayer* textLayer;

static AppTimer* timer;

static DarkValue dark;

static MonsterPart allyPart = {.previous = 0};
static MonsterPart enemyPart = {.previous = 0};

static uint16_t allyHealth;
static uint16_t enemyHealth;

static char textBuffer[8];

static void end() {
  window_stack_remove(window, true);
}

static void event(char* text, int endTimeout) {
  snprintf(textBuffer, 8, text);
  text_layer_set_text(textLayer, textBuffer);
  if (timer) {
    app_timer_cancel(timer);
  }
  timer = app_timer_register(endTimeout, end, NULL);
}

static bool start() {
  window_stack_push(window, true);
  DictionaryIterator* outbox;
  app_message_outbox_begin(&outbox);
  if (!outbox) {
    return false;
  }
  dict_write_cstring(outbox, MESSAGE_KEY_mp_steps,  "");
  dict_write_cstring(outbox, MESSAGE_KEY_mp_sleep,  "");
  dict_write_cstring(outbox, MESSAGE_KEY_mp_restful,  "");
  dict_write_cstring(outbox, MESSAGE_KEY_mp_active, "");
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
  }
  tuple = dict_find(iter, MESSAGE_KEY_mp_enemy_health);
  if (tuple) {
    enemyHealth = tuple->value->uint16;
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
}

static void window_unload(Window* window) {
  monster_unload(&allyPart);
  monster_unload(&enemyPart);
  text_layer_destroy(textLayer);
  bitmap_layer_destroy(templateLayer);
  layer_destroy(root);
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
      if (start()) {
        event("WAITING", 60000);
      } else {
        event("ERROR", 15000);
      }
      break;
    case MP_FIGHT:
      event("", 30000);
      break;
    case MP_LOST:
      event("LOST", 15000);
      break;
    case MP_WON:
      event("WON", 15000);
      break;
  }
  update(iter);
}

void multiplayer_deinit() {
  window_destroy(window);
}
