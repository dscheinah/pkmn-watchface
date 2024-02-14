#pragma once
#include <pebble.h>
#include "../state/global.h"

typedef struct {
  BitmapLayer* image;
  GBitmap* bitmap;
  TextLayer* level;
  Layer* health;
  ResourceValue previous;
} MonsterPart;

void monster_init(State* state);

BitmapLayer* monster_create_template(Layer* root);

void monster_load_ally(Layer* root, MonsterPart* part, DarkValue dark);

void monster_load_enemy(Layer* root, MonsterPart* part, DarkValue dark);

void monster_render(MonsterPart* part, ResourceValue resource, DarkValue dark);

void monster_unload(MonsterPart* part);

void monster_deinit();
