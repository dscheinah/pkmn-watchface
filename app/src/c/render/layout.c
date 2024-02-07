#include <pebble.h>
#include "layout.h"
#include "cache.h"
#include "helper.h"
#include "monster.h"
#include "window.h"

static Layer* root;
static Layer* watchLayer;
static BitmapLayer* templateLayer;

void layout_load(Window* window, State* state) {
  root = window_create_center_layer(window);
  templateLayer = monster_create_template(root);
  if (state->settings & SETTINGS_CACHE) {
    cache_layer_create(root, bitmap_layer_get_layer(templateLayer));
  }
  watchLayer = helper_create_layer(root, layer_get_bounds(root));
}

Layer* layout_get_battlefield() {
  return bitmap_layer_get_layer(templateLayer);
}

Layer* layout_get_watch() {
  return watchLayer;
}

void layout_unload() {
  cache_layer_destroy();
  bitmap_layer_destroy(templateLayer);
  layer_destroy(watchLayer);
  layer_destroy(root);
}
