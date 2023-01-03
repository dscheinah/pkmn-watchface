#include <pebble.h>
#include "cache.h"
#include "helper.h"

static Layer* cacheLayer;
static Layer* captureLayer;
static Layer* replacementLayer;
static GBitmap* cacheBitmap;
static GRect bounds;
static GBitmapDataRowInfo sourceRow;
static GBitmapDataRowInfo destinationRow;

static void copy(GBitmap* source, GBitmap* destination) {
  for (int i = 0; i < bounds.size.h; i++) {
    sourceRow = gbitmap_get_data_row_info(source, i);
    destinationRow = gbitmap_get_data_row_info(destination, i);
    #if defined(PBL_COLOR)
      memcpy(destinationRow.data, sourceRow.data, sourceRow.max_x + 1);
    #else
      memcpy(destinationRow.data, sourceRow.data, sourceRow.max_x / 8 + 1);
    #endif
  }
}

static void update(Layer* layer, GContext* ctx) {
  if (layer_get_hidden(replacementLayer) && !graphics_frame_buffer_is_captured(ctx)) {
    GBitmap* buffer = graphics_capture_frame_buffer(ctx);
    copy(cacheBitmap, buffer);
    graphics_release_frame_buffer(ctx, buffer);
  }
}

static void capture(Layer* layer, GContext* ctx) {
  if (graphics_frame_buffer_is_captured(ctx)) {
    return;
  }
  GBitmap* buffer = graphics_capture_frame_buffer(ctx);
  copy(buffer, cacheBitmap);
  graphics_release_frame_buffer(ctx, buffer);

  layer_set_hidden(replacementLayer, true);
  layer_set_hidden(captureLayer, true);
}

void cache_layer_create(Layer* root, Layer* replacement) {
  replacementLayer = replacement;

  bounds = layer_get_bounds(root);

  cacheLayer = helper_create_layer(root, bounds);
  layer_set_update_proc(cacheLayer, update);

  captureLayer = helper_create_layer(cacheLayer, bounds);
  layer_set_update_proc(captureLayer, capture);

  #if defined(PBL_COLOR)
    cacheBitmap = gbitmap_create_blank(bounds.size, GBitmapFormat8Bit);
  #else
    cacheBitmap = gbitmap_create_blank(bounds.size, GBitmapFormat1Bit);
  #endif
}

void cache_layer_mark_dirty() {
  if (replacementLayer) {
    layer_set_hidden(replacementLayer, false);
    layer_set_hidden(captureLayer, false);
  }
}

void cache_layer_destroy() {
  gbitmap_destroy(cacheBitmap);
  layer_destroy(captureLayer);
  layer_destroy(cacheLayer);
  replacementLayer = NULL;
}
