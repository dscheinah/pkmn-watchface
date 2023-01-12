#include <pebble.h>
#include "watch.h"
#include "helper.h"

static State* state;

static TextLayer* timeLayer;
static TextLayer* dateLayer;
static TextLayer* secondsLayer;
static TextLayer* statusLayer;

static char* timeFormat;
static char* dateFormat;

static char timeBuffer[6];
static char dateBuffer[13];
static char statusBuffer[4];
static char secondsBuffer[3];

void watch_load(Layer* root, Layer* cachedRoot, State* stateRef) {
  state = stateRef;
  timeFormat = state->settings & SETTINGS_TIME_FORMAT ? "%H:%M" : "%I:%M";
  dateFormat = state->settings & SETTINGS_DATE_FORMAT ? "%d / %m" : "%m / %d";
  if (state->settings & SETTINGS_SECONDS) {
    secondsLayer = helper_create_text_layer(root, GRect(110, PBL_IF_ROUND_ELSE(125, 133), 28, 20), FONT_MEDIUM, GTextAlignmentLeft);
    timeLayer = helper_create_text_layer(root, GRect(13, 121, 97, 32), FONT_LARGE, GTextAlignmentCenter);
  } else {
    timeLayer = helper_create_text_layer(root, GRect(9, 121, 129, 32), FONT_LARGE, GTextAlignmentCenter);
  }
  dateLayer = helper_create_text_layer(cachedRoot, GRect(73, 95, 59, 14), FONT_SMALL, GTextAlignmentRight);
  text_layer_set_background_color(dateLayer, GColorClear);
  if (state->settings & SETTINGS_DOW) {
    statusLayer = helper_create_text_layer(cachedRoot, GRect(101, 73, 35, 14), FONT_SMALL, GTextAlignmentRight);
  }
}

void watch_render_time(struct tm* tick_time) {
  strftime(timeBuffer, 6, timeFormat, tick_time);
  text_layer_set_text(timeLayer, timeBuffer);
}

void watch_render_date(struct tm* tick_time) {
  strftime(dateBuffer, 13, dateFormat, tick_time);
  text_layer_set_text(dateLayer, dateBuffer);
  if (statusLayer) {
    strftime(statusBuffer, 4, "%a", tick_time);
    statusBuffer[1] -= 32;
    statusBuffer[2] -= 32;
    text_layer_set_text(statusLayer, statusBuffer);
  }
}

void watch_render_seconds(struct tm* tick_time) {
  strftime(secondsBuffer, 3, "%S", tick_time);
  if (secondsLayer) {
    text_layer_set_text(secondsLayer, secondsBuffer);
  } else if (state->settings & SETTINGS_TAPS) {
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
