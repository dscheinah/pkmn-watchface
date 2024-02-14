#include <pebble.h>
#include "window.h"
#include "helper.h"

#define X (PBL_DISPLAY_WIDTH - 144) / 2
#define Y (PBL_DISPLAY_HEIGHT - 168) / 2

Window* window_create_custom(State* state, WindowHandlers handlers) {
  Window* window = window_create();
  if (state->settings & SETTINGS_DARK ? DARK_FULL : DARK_OFF) {
    window_set_background_color(window, COLOR_FALLBACK(GColorDarkGray, GColorBlack));
  }
  window_set_window_handlers(window, handlers);
  return window;
}

Layer* window_create_center_layer(Window* window) {
  return helper_create_layer(window_get_root_layer(window), GRect(X, Y, 144, 168));
}
