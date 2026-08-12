#pragma once
#include <pebble.h>
#include "../state/global.h"

#if PBL_DISPLAY_HEIGHT == 228
    #define WIDTH 195
    #define HEIGHT 228
    #define OFFSET 136 / 100

    #define FONT_LARGE      FONT_KEY_LECO_42_NUMBERS
    #define FONT_MEDIUM     FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM
    #define FONT_SMALL      FONT_KEY_GOTHIC_18
    #define FONT_SMALL_BOLD FONT_KEY_GOTHIC_18_BOLD
#else
    #define WIDTH 144
    #define HEIGHT 168
    #define OFFSET 1

    #define FONT_LARGE      FONT_KEY_LECO_32_BOLD_NUMBERS
    #define FONT_MEDIUM     FONT_KEY_LECO_20_BOLD_NUMBERS
    #define FONT_SMALL      FONT_KEY_GOTHIC_14
    #define FONT_SMALL_BOLD FONT_KEY_GOTHIC_14_BOLD
#endif

#define ALIGN_LEFT  0
#define ALIGN_RIGHT 1

TextLayer* helper_create_text_layer(Layer* parent, GRect rect, char* font, GTextAlignment alignment, DarkValue dark);

BitmapLayer* helper_create_bitmap_layer(Layer* parent, GRect rect, GBitmap* bitmap);

Layer* helper_create_layer(Layer* parent, GRect rect);

GBitmap* helper_create_bitmap(ResourceValue resource, DarkValue dark);

void helper_render_percentage_rect(Layer* layer, GContext* ctx, int alignment, GColor8 color, int percentage);

GColor8 helper_color_from_health(int percentage);

GRect GRectOffset(int x, int y, int w, int h);
