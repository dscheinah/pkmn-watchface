#include <pebble.h>
#include "layout.h"
#include "cache.h"
#include "helper.h"

static Layer* watchLayer;
static BitmapLayer* templateLayer;
static GBitmap* templateBitmap;

void layout_load(Window* window, State* state) {
  window_set_background_color(window, COLOR_FALLBACK(GColorDarkGray, GColorBlack));

  Layer* root = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root);

  int x = (bounds.size.w - 144) / 2;
  int y = (bounds.size.h - 168) / 2;
  GRect coords = GRect(x, y, 144, 168);

  templateBitmap = helper_create_bitmap(RESOURCE_ID_template, true);
  templateLayer = helper_create_bitmap_layer(root, coords, templateBitmap);

  if (state->settings & SETTINGS_CACHE) {
    cache_layer_create(root, bitmap_layer_get_layer(templateLayer));
  }

  watchLayer = helper_create_layer(root, coords);
}

Layer* layout_get_root() {
  return bitmap_layer_get_layer(templateLayer);
}

Layer* layout_get_battlefield() {
  return bitmap_layer_get_layer(templateLayer);
}

Layer* layout_get_watch() {
  return watchLayer;
}

void layout_unload() {
  cache_layer_destroy();
  gbitmap_destroy(templateBitmap);
  bitmap_layer_destroy(templateLayer);
  layer_destroy(watchLayer);
}
