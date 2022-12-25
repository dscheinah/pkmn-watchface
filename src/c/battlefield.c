#include <pebble.h>
#include "battlefield.h"
#include "cache.h"
#include "const.h"
#include "helper.h"
#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1

typedef struct {
  Ally* ally;
  Enemy* enemy;
  BitmapLayer* image;
  GBitmap* bitmap;
  TextLayer* level;
  Layer* health;
  Layer* experience;
  bool missing;
  int previous;
} Part;

static Part allyPart = {.previous = 0};
static Part enemyPart = {.previous = 0, .missing = 0};

static EventValue *current;
static Layer *indicator;

static void renderRect(Layer *layer, GContext *ctx, int alignment, GColor8 color, int percentage) {
  GRect bounds = layer_get_bounds(layer);
  #if defined(PBL_COLOR)
    graphics_context_set_fill_color(ctx, color);
  #else
    graphics_context_set_fill_color(ctx, GColorLightGray);
  #endif
  int w = bounds.size.w * percentage / 100;
  int x = alignment == ALIGN_LEFT ? 0 : bounds.size.w - w;
  graphics_fill_rect(ctx, GRect(x, 0, w, bounds.size.h), 0, GCornerNone);
}

static void renderCircle(GContext *ctx, GColor8 color, int pos) {
  #if defined(PBL_COLOR)
    graphics_context_set_stroke_color(ctx, color);
  #else
    graphics_context_set_stroke_color(ctx, GColorBlack);
  #endif
  graphics_draw_round_rect(ctx, GRect(10 * pos, 0, 7, 4), 4);
}

static void renderBitmap(Part *part, int resource) {
  if (part->previous == resource) {
    return;
  }
  part->previous = resource;
  gbitmap_destroy(part->bitmap);
  part->bitmap = gbitmap_create_with_resource(resource);
  bitmap_layer_set_bitmap(part->image, part->bitmap);
}

static GColor8 GColorFromHealth(int percentage) {
  return GColorFromRGB((100 - percentage) * 255 / 100, percentage * 255 / 100, 0);
}

static void renderAllyExperience(Layer *layer, GContext *ctx) {
  renderRect(layer, ctx, ALIGN_RIGHT, GColorBlue, allyPart.ally->experience);
}

static void renderAllyHealth(Layer *layer, GContext *ctx) {
  renderRect(layer, ctx, ALIGN_LEFT, GColorFromHealth(allyPart.ally->health), allyPart.ally->health);
}

static void renderEnemyHealth(Layer *layer, GContext *ctx) {
  renderRect(layer, ctx, ALIGN_LEFT, GColorFromHealth(enemyPart.enemy->health), enemyPart.enemy->health);
}

static void renderEnemyExperience(Layer *layer, GContext *ctx) {
  renderRect(layer, ctx, ALIGN_LEFT, GColorVeryLightBlue, 100 * enemyPart.enemy->index_count / ENEMY_COUNT);
}

static void renderIndicator(Layer *layer, GContext *ctx) {
  graphics_context_set_antialiased(ctx, false);
  if (*current & EVENT_SLEEP) {
    renderCircle(ctx, GColorSunsetOrange, 0);
  }
  if (*current & EVENT_EGG) {
    renderCircle(ctx, GColorMidnightGreen, 1);
  }
  if (*current & EVENT_GHOST) {
    renderCircle(ctx, GColorBlueMoon, 2);
  }
  if (*current & EVENT_MORPH) {
    renderCircle(ctx, GColorPurpureus, 3);
  }
  if (*current & EVENT_EVO) {
    renderCircle(ctx, GColorLiberty, 4);
  }
}

void battlefield_load(Layer *root, Ally *ally, Enemy *enemy, EventValue *event) {
  allyPart.ally = ally;
  enemyPart.enemy = enemy;
  current = event;

  allyPart.image = bitmap_layer_create(GRect(10, 68, 48, 48));
  allyPart.level = helper_create_text_layer(GRect(72, 73, 25, 14), FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
  allyPart.health = layer_create(GRect(96, 91, 37, 4));
  allyPart.experience = layer_create(GRect(76, 111, 61, 2));
  layer_add_child(root, bitmap_layer_get_layer(allyPart.image));
  layer_add_child(root, text_layer_get_layer(allyPart.level));
  layer_add_child(root, allyPart.health);
  layer_add_child(root, allyPart.experience);

  enemyPart.image = bitmap_layer_create(GRect(82, 14, 56, 56));
  enemyPart.level = helper_create_text_layer(GRect(16, 10, 25, 14), FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
  enemyPart.health = layer_create(GRect(40, 28, 29, 3));
  enemyPart.experience = layer_create(GRect(20, 41, 49, 1));
  bitmap_layer_set_alignment(enemyPart.image, GAlignBottom);
  layer_add_child(root, bitmap_layer_get_layer(enemyPart.image));
  layer_add_child(root, text_layer_get_layer(enemyPart.level));
  layer_add_child(root, enemyPart.health);
  layer_add_child(root, enemyPart.experience);

  indicator = layer_create(GRect(21, 35, 47, 4));
  layer_add_child(root, indicator);

  layer_set_update_proc(allyPart.health, renderAllyHealth);
  layer_set_update_proc(allyPart.experience, renderAllyExperience);
  layer_set_update_proc(enemyPart.health, renderEnemyHealth);
  layer_set_update_proc(enemyPart.experience, renderEnemyExperience);
  layer_set_update_proc(indicator, renderIndicator);
}

void battlefield_set_enemy_missing(bool missing) {
  if (enemyPart.missing ^ missing) {
    enemyPart.missing = missing;
    battlefield_mark_dirty();
  }
}

void battlefield_mark_dirty() {
  cache_layer_mark_dirty();

  int type = quiet_time_is_active() ? allyPart.ally->type + ENEMY_COUNT + ENEMY_OFFSET - 1 : allyPart.ally->type;
  renderBitmap(&allyPart, allyPart.ally->shiny ? type + 10 : type);
  renderBitmap(&enemyPart, enemyPart.missing ? RESOURCE_ID_0 : enemyPart.enemy->type);

  static char allyLevelBuffer[5];
  int level = allyPart.ally->level_final();
  snprintf(allyLevelBuffer, 5, "L%d", level > 100 ? level - 100 : level);
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
  layer_destroy(enemyPart.experience);
  layer_destroy(indicator);
}
