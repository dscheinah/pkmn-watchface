#include "helper.h"

TextLayer* helper_create_text_layer(GRect rect, char *font, GTextAlignment alignment) {
  TextLayer *layer = text_layer_create(rect);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_font(layer, fonts_get_system_font(font));
  text_layer_set_text_alignment(layer, alignment);
  return layer;
}
