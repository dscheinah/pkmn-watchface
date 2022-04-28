#include <pebble.h>
#include "watch.h"
#include "helper.h"
#define VERSION 1
#define VERSION_KEY 500
#define FLAGS_KEY 501

static uint current = WATCH_DOW | WATCH_BLUETOOTH;

static TextLayer *timeLayer, *dateLayer, *secondsLayer, *statusLayer;
static char *timeFormat, *dateFormat;

static void updateTimeFormat() {
  if (clock_is_24h_style()) {
    current |= WATCH_TIME_FORMAT;
  } else {
    current &= ~WATCH_TIME_FORMAT;
  }
  timeFormat = current & WATCH_TIME_FORMAT ? "%H:%M" : "%I:%M";
  dateFormat = current & WATCH_DATE_FORMAT ? "%d / %m" : "%m / %d";
}

void watch_init() {
  if (persist_exists(VERSION_KEY) && persist_read_int(VERSION_KEY) == VERSION) {
    current = persist_read_int(FLAGS_KEY);
  }
  updateTimeFormat();
}

void watch_load(Layer *root, Layer *cachedRoot) {
  if (current & WATCH_SECONDS) {
    secondsLayer = helper_create_text_layer(GRect(110, 133, 28, 20), FONT_KEY_LECO_20_BOLD_NUMBERS, GTextAlignmentLeft);
    layer_add_child(root, text_layer_get_layer(secondsLayer));
    timeLayer = helper_create_text_layer(GRect(9, 121, 101, 32), FONT_KEY_LECO_32_BOLD_NUMBERS, GTextAlignmentCenter);
  } else {
    timeLayer = helper_create_text_layer(GRect(9, 121, 129, 32), FONT_KEY_LECO_32_BOLD_NUMBERS, GTextAlignmentCenter);
  }
  dateLayer = helper_create_text_layer(GRect(73, 95, 59, 14), FONT_KEY_GOTHIC_14, GTextAlignmentRight);
  text_layer_set_background_color(dateLayer, GColorClear);
  layer_add_child(root, text_layer_get_layer(timeLayer));
  layer_add_child(cachedRoot, text_layer_get_layer(dateLayer));

  if (current & WATCH_DOW) {
    statusLayer = helper_create_text_layer(GRect(101, 73, 35, 14), FONT_KEY_GOTHIC_14, GTextAlignmentRight);
    layer_add_child(cachedRoot, text_layer_get_layer(statusLayer));
  }
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

void watch_render_seconds(struct tm *tick_time) {
  static char secondsBuffer[3];
  strftime(secondsBuffer, 3, "%S", tick_time);
  if (secondsLayer) {
    text_layer_set_text(secondsLayer, secondsBuffer);
  } else if (current & WATCH_TAPS) {
    text_layer_set_text(timeLayer, secondsBuffer);
  }
}

void watch_set_settings(uint flags) {
  current = flags;
  updateTimeFormat();
  time_t now = time(NULL);
  watch_render_date(localtime(&now));
}

bool watch_has_seconds() {
  if (current & WATCH_TAPS) {
    return false;
  }
  return current & WATCH_SECONDS;
}

bool watch_has_bluetooth() {
  return current & WATCH_BLUETOOTH;
}

bool watch_has_taps() {
  return current & WATCH_TAPS;
}

void watch_unload() {
  text_layer_destroy(timeLayer);
  text_layer_destroy(dateLayer);
  text_layer_destroy(secondsLayer);
  text_layer_destroy(statusLayer);
  secondsLayer = NULL;
  statusLayer = NULL;
}

void watch_deinit() {
  persist_write_int(VERSION_KEY, VERSION);
  persist_write_int(FLAGS_KEY, current);
}
