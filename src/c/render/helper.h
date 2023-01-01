#pragma once
#include <pebble.h>

#define FONT_LARGE      FONT_KEY_LECO_32_BOLD_NUMBERS
#define FONT_MEDIUM     FONT_KEY_LECO_20_BOLD_NUMBERS
#define FONT_SMALL      FONT_KEY_GOTHIC_14
#define FONT_SMALL_BOLD FONT_KEY_GOTHIC_14_BOLD

TextLayer* helper_create_text_layer(Layer* parent, GRect rect, char* font, GTextAlignment alignment);

BitmapLayer* helper_create_bitmap_layer(Layer* parent, GRect rect, GBitmap* bitmap);

Layer* helper_create_layer(Layer* parent, GRect rect);
