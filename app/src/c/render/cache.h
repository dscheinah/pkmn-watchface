#pragma once
#include <pebble.h>

void cache_layer_create(Layer* root, Layer* replacement);

void cache_layer_mark_dirty();

void cache_layer_destroy();
