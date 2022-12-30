#include <pebble.h>
#include "cache.h"
#include "helper.h"

static Layer* watchLayer;
static BitmapLayer* templateLayer;
static GBitmap* templateBitmap;

void layout_load(Layer* root) {
  GRect bounds = layer_get_bounds(root);

  int x = (bounds.size.w - 144) / 2;
  int y = (bounds.size.h - 168) / 2;
  GRect coords = GRect(x, y, 144, 168);

  templateLayer = bitmap_layer_create(coords);
  templateBitmap = gbitmap_create_with_resource(RESOURCE_ID_template);
  bitmap_layer_set_bitmap(templateLayer, templateBitmap);

  cache_layer_create(root, bitmap_layer_get_layer(templateLayer));

  watchLayer = layer_create(coords);
  layer_add_child(root, watchLayer);
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
