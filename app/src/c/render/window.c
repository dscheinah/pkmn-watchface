#include <pebble.h>
#include "window.h"
#include "helper.h"

#define X (PBL_DISPLAY_WIDTH - WIDTH) / 2
#define Y (PBL_DISPLAY_HEIGHT - HEIGHT) / 2

Window* window_create_custom(State* state, WindowHandlers handlers) {
  Window* window = window_create();
  if (state->settings & SETTINGS_DARK) {
    window_set_background_color(window, GColorBlack);
  }
  window_set_window_handlers(window, handlers);
  return window;
}

Layer* window_create_center_layer(Window* window) {
  return helper_create_layer(window_get_root_layer(window), GRect(X, Y, WIDTH, HEIGHT));
}
