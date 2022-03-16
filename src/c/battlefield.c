#include <pebble.h>
#include "battlefield.h"
#include "helper.h"
#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1

struct part {
  Ally* ally;
  Enemy* enemy;
  BitmapLayer* image;
  GBitmap* bitmap;
  TextLayer* level;
  Layer* health;
  Layer* experience;
  bool missing;
  int previous;
};

static struct part allyPart;
static struct part enemyPart;

static void renderRect(Layer *layer, GContext *ctx, int alignment, GColor8 color, float percentage) {
  GRect bounds = layer_get_bounds(layer);
  #if defined(PBL_COLOR)
    graphics_context_set_fill_color(ctx, color);
  #else
    graphics_context_set_fill_color(ctx, GColorLightGray);
  #endif
  int w = bounds.size.w * percentage;
  int x = alignment == ALIGN_LEFT ? 0 : bounds.size.w - w;
  graphics_fill_rect(ctx, GRect(x, 0, w, bounds.size.h), 0, GCornerNone);
}

static void renderBitmap(BitmapLayer *layer, GBitmap *bitmap, int resource) {
  gbitmap_destroy(bitmap);
  bitmap = gbitmap_create_with_resource(resource);
  bitmap_layer_set_bitmap(layer, bitmap);
}

static GColor8 GColorFromHealth(float percentage) {
  return GColorFromRGB((1 - percentage) * 255, percentage * 255, 0);
}

static void renderExperience(Layer *layer, GContext *ctx) {
  renderRect(layer, ctx, ALIGN_RIGHT, GColorBlue, allyPart.ally->experience);
}

static void renderAllyHealth(Layer *layer, GContext *ctx) {
  float percentage = (float) allyPart.ally->health / 100;
  renderRect(layer, ctx, ALIGN_LEFT, GColorFromHealth(percentage), percentage);
}

static void renderEnemyHealth(Layer *layer, GContext *ctx) {
  float percentage = (float) enemyPart.enemy->health / 100;
  renderRect(layer, ctx, ALIGN_LEFT, GColorFromHealth(percentage), percentage);
}

void battlefield_load(Layer *root, Ally *ally, Enemy *enemy) {
  allyPart.ally = ally;
  allyPart.previous = 0;
  enemyPart.enemy = enemy;
  enemyPart.missing = 0;
  enemyPart.previous = 0;

  allyPart.image = bitmap_layer_create(GRect(10, 68, 48, 48));
  allyPart.level = helper_create_text_layer(GRect(72, 73, 25, 14), FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
  allyPart.health = layer_create(GRect(96, 91, 37, 4));
  allyPart.experience = layer_create(GRect(76, 111, 61, 2));
  layer_add_child(root, bitmap_layer_get_layer(allyPart.image));
  layer_add_child(root, text_layer_get_layer(allyPart.level));
  layer_add_child(root, allyPart.health);
  layer_add_child(root, allyPart.experience);

  enemyPart.image = bitmap_layer_create(GRect(82, 12, 56, 56));
  enemyPart.level = helper_create_text_layer(GRect(16, 10, 25, 14), FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
  enemyPart.health = layer_create(GRect(40, 28, 29, 3));
  bitmap_layer_set_alignment(enemyPart.image, GAlignBottom);
  layer_add_child(root, bitmap_layer_get_layer(enemyPart.image));
  layer_add_child(root, text_layer_get_layer(enemyPart.level));
  layer_add_child(root, enemyPart.health);

  layer_set_update_proc(allyPart.experience, renderExperience);
  layer_set_update_proc(allyPart.health, renderAllyHealth);
  layer_set_update_proc(enemyPart.health, renderEnemyHealth);
}

void battlefield_set_enemy_missing(bool missing) {
  if (enemyPart.missing ^ missing) {
    enemyPart.missing = missing;
    battlefield_mark_dirty();
  }
}

void battlefield_mark_dirty() {
  int allyToRender = allyPart.ally->shiny ? allyPart.ally->type + 9 : allyPart.ally->type;
  if (allyPart.previous != allyToRender) {
    allyPart.previous = allyToRender;
    renderBitmap(allyPart.image, allyPart.bitmap, allyPart.previous);
  }
  int enemyToRender = enemyPart.missing ? RESOURCE_ID_0 : enemyPart.enemy->type;
  if (enemyPart.previous != enemyToRender) {
    enemyPart.previous = enemyToRender;
    renderBitmap(enemyPart.image, enemyPart.bitmap, enemyToRender);
  }
  static char allyLevelBuffer[5];
  snprintf(allyLevelBuffer, 5, "L%d", allyPart.ally->level_final());
  text_layer_set_text(allyPart.level, allyLevelBuffer);
  static char enemyLevelBuffer[5];
  snprintf(enemyLevelBuffer, 5, "L%d", enemyPart.enemy->level_final());
  text_layer_set_text(enemyPart.level, enemyLevelBuffer);
}

void battlefield_unload() {
  gbitmap_destroy(allyPart.bitmap);
  gbitmap_destroy(enemyPart.bitmap);

  bitmap_layer_destroy(allyPart.image);
  text_layer_destroy(allyPart.level);
  layer_destroy(allyPart.health);
  layer_destroy(allyPart.experience);

  bitmap_layer_destroy(enemyPart.image);
  text_layer_destroy(enemyPart.level);
  layer_destroy(enemyPart.health);
}
