#include <pebble.h>
#include "multiplayer.h"
#include "../render/monster.h"
#include "../render/window.h"

static Window* window;

static Layer* root;
static BitmapLayer* templateLayer;

static AppTimer* timer;

static void window_load(Window* window) {
  root = window_create_center_layer(window);
  templateLayer = monster_create_template(root);
}

static void window_unload(Window* window) {
  bitmap_layer_destroy(templateLayer);
  layer_destroy(root);
}

static void start() {
  window_stack_push(window, true);
}

static void end() {
  window_stack_remove(window, true);
}

void multiplayer_init(State* state) {
  window = window_create_custom(state, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
}

void multiplayer_handle_inbox(DictionaryIterator* iter) {
  start();
  if (timer) {
    app_timer_cancel(timer);
  }
  timer = app_timer_register(60000, end, NULL);
}

void multiplayer_deinit() {
  window_destroy(window);
}
