#include <pebble.h>
#include "helper.h"

TextLayer* helper_create_text_layer(Layer* parent, GRect rect, char* font, GTextAlignment alignment) {
  TextLayer* layer = text_layer_create(rect);
  layer_add_child(parent, text_layer_get_layer(layer));
  text_layer_set_font(layer, fonts_get_system_font(font));
  text_layer_set_text_alignment(layer, alignment);
  return layer;
}

BitmapLayer* helper_create_bitmap_layer(Layer* parent, GRect rect, GBitmap* bitmap) {
  BitmapLayer* layer = bitmap_layer_create(rect);
  layer_add_child(parent, bitmap_layer_get_layer(layer));
  if (bitmap != NULL) {
    bitmap_layer_set_bitmap(layer, bitmap);
  }
  return layer;
}

Layer* helper_create_layer(Layer* parent, GRect rect) {
  Layer* layer = layer_create(rect);
  layer_add_child(parent, layer);
  return layer;
}
