#include <pebble.h>
#include "helper.h"

TextLayer* helper_create_text_layer(Layer* parent, GRect rect, char* font, GTextAlignment alignment, DarkValue dark) {
  TextLayer* layer = text_layer_create(rect);
  layer_add_child(parent, text_layer_get_layer(layer));
  text_layer_set_font(layer, fonts_get_system_font(font));
  text_layer_set_text_alignment(layer, alignment);
  if (dark) {
    text_layer_set_text_color(layer, GColorWhite);
    text_layer_set_background_color(layer, GColorBlack);
  }
  return layer;
}

BitmapLayer* helper_create_bitmap_layer(Layer* parent, GRect rect, GBitmap* bitmap) {
  BitmapLayer* layer = bitmap_layer_create(rect);
  bitmap_layer_set_alignment(layer, GAlignBottom);
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

GBitmap* helper_create_bitmap(ResourceValue resource, DarkValue dark) {
  GBitmap* bitmap = gbitmap_create_with_resource(resource);
  if (dark) {
    GColor* palette = gbitmap_get_palette(bitmap);
    int length = ARRAY_LENGTH(palette);
    for (int i = 0; i < length; i++) {
      if (gcolor_equal(palette[i], GColorWhite)) {
        palette[i] = GColorBlack;
        continue;
      }
      if (gcolor_equal(palette[i], GColorBlack)) {
        palette[i] = GColorWhite;
      }
    }
  }
  return bitmap;
}

void helper_render_percentage_rect(Layer* layer, GContext* ctx, int alignment, GColor8 color, int percentage) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, COLOR_FALLBACK(color, GColorLightGray));
  int w = bounds.size.w * percentage / 100;
  int x = alignment == ALIGN_LEFT ? 0 : bounds.size.w - w;
  graphics_fill_rect(ctx, GRect(x, 0, w, bounds.size.h), 0, GCornerNone);
}

GColor8 helper_color_from_health(int percentage) {
  if (percentage > 46 && percentage < 57) {
    return GColorChromeYellow;
  }
  return GColorFromRGB((100 - percentage) * 255 / 100, percentage * 255 / 100, 0);
}
