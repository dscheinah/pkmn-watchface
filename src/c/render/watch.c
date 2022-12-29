#include <pebble.h>
#include "watch.h"
#include "helper.h"
#include "../state/settings.h"

static TextLayer *timeLayer, *dateLayer, *secondsLayer, *statusLayer;
static char *timeFormat, *dateFormat;

void watch_load(Layer *root, Layer *cachedRoot, int settings) {
  if (settings & SETTINGS_SECONDS) {
    #if defined(PBL_ROUND)
      secondsLayer = helper_create_text_layer(GRect(110, 125, 28, 20), FONT_KEY_LECO_20_BOLD_NUMBERS, GTextAlignmentLeft);
    #else
      secondsLayer = helper_create_text_layer(GRect(110, 133, 28, 20), FONT_KEY_LECO_20_BOLD_NUMBERS, GTextAlignmentLeft);
    #endif
    layer_add_child(root, text_layer_get_layer(secondsLayer));
    timeLayer = helper_create_text_layer(GRect(13, 121, 97, 32), FONT_KEY_LECO_32_BOLD_NUMBERS, GTextAlignmentCenter);
  } else {
    timeLayer = helper_create_text_layer(GRect(9, 121, 129, 32), FONT_KEY_LECO_32_BOLD_NUMBERS, GTextAlignmentCenter);
  }
  dateLayer = helper_create_text_layer(GRect(73, 95, 59, 14), FONT_KEY_GOTHIC_14, GTextAlignmentRight);
  text_layer_set_background_color(dateLayer, GColorClear);
  layer_add_child(root, text_layer_get_layer(timeLayer));
  layer_add_child(cachedRoot, text_layer_get_layer(dateLayer));

  if (settings & SETTINGS_DOW) {
    statusLayer = helper_create_text_layer(GRect(101, 73, 35, 14), FONT_KEY_GOTHIC_14, GTextAlignmentRight);
    layer_add_child(cachedRoot, text_layer_get_layer(statusLayer));
  }

  watch_set_settings(settings);
}

void watch_set_settings(int settings) {
  timeFormat = settings & SETTINGS_TIME_FORMAT ? "%H:%M" : "%I:%M";
  dateFormat = settings & SETTINGS_DATE_FORMAT ? "%d / %m" : "%m / %d";
  time_t now = time(NULL);
  watch_render_date(localtime(&now));
}

void watch_render_time(struct tm *tick_time) {
  static char timeBuffer[6];
  strftime(timeBuffer, 6, timeFormat, tick_time);
  text_layer_set_text(timeLayer, timeBuffer);
}

void watch_render_date(struct tm *tick_time) {
  static char dateBuffer[13];
  strftime(dateBuffer, 13, dateFormat, tick_time);
  text_layer_set_text(dateLayer, dateBuffer);
  if (statusLayer) {
    static char statusBuffer[4];
    strftime(statusBuffer, 4, "%a", tick_time);
    statusBuffer[1] -= 32;
    statusBuffer[2] -= 32;
    text_layer_set_text(statusLayer, statusBuffer);
  }
}

void watch_render_seconds(struct tm *tick_time, int settings) {
  static char secondsBuffer[3];
  strftime(secondsBuffer, 3, "%S", tick_time);
  if (secondsLayer) {
    text_layer_set_text(secondsLayer, secondsBuffer);
  } else if (settings & SETTINGS_TAPS) {
    text_layer_set_text(timeLayer, secondsBuffer);
  }
}

void watch_unload() {
  text_layer_destroy(timeLayer);
  text_layer_destroy(dateLayer);
  text_layer_destroy(secondsLayer);
  text_layer_destroy(statusLayer);
  secondsLayer = NULL;
  statusLayer = NULL;
}
