#include <pebble.h>
#include "monster.h"
#include "helper.h"

static GBitmap* templateBitmap;

void monster_init(State* state) {
  templateBitmap = helper_create_bitmap(RESOURCE_ID_template, state->settings & SETTINGS_DARK ? DARK_FULL : DARK_OFF);
}

BitmapLayer* monster_create_template(Layer* root) {
  return helper_create_bitmap_layer(root, GRect(0, 0, 144, 168), templateBitmap);
}

void monster_load_ally(Layer* root, MonsterPart* part, DarkValue dark) {
  part->image = helper_create_bitmap_layer(root, GRect(10, 68, 48, 48), NULL);
  part->level = helper_create_text_layer(root, GRect(72, 73, 25, 14), FONT_SMALL_BOLD, GTextAlignmentCenter, dark);
  part->health = helper_create_layer(root, GRect(96, 91, 37, 4));
}

void monster_load_enemy(Layer* root, MonsterPart* part, DarkValue dark) {
  part->image = helper_create_bitmap_layer(root, GRect(82, 14, 56, 56), NULL);
  part->level = helper_create_text_layer(root, GRect(16, 10, 25, 14), FONT_SMALL_BOLD, GTextAlignmentCenter, dark);
  part->health = helper_create_layer(root, GRect(40, 28, 29, 3));
}

void monster_render(MonsterPart* part, ResourceValue resource, DarkValue dark) {
  if (part->previous == resource) {
    return;
  }
  part->previous = resource;
  gbitmap_destroy(part->bitmap);
  part->bitmap = helper_create_bitmap(resource, dark);
  bitmap_layer_set_bitmap(part->image, part->bitmap);
}

void monster_unload(MonsterPart* part) {
  gbitmap_destroy(part->bitmap);
  bitmap_layer_destroy(part->image);
  text_layer_destroy(part->level);
  layer_destroy(part->health);
  part->previous = 0;
}

void monster_deinit() {
  gbitmap_destroy(templateBitmap);
}
