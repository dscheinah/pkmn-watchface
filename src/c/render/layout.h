#pragma once
#include <pebble.h>

void layout_load(Layer* root);

Layer* layout_get_root();

Layer* layout_get_battlefield();

Layer* layout_get_watch();

void layout_unload();
