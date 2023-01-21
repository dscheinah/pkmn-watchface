#include <pebble.h>
#include "battlefield.h"
#include "cache.h"
#include "helper.h"
#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1

typedef struct {
  BitmapLayer* image;
  GBitmap* bitmap;
  TextLayer* level;
  Layer* health;
  Layer* experience;
  ResourceValue previous;
} Part;

static State* state;
static Part allyPart = {.previous = 0};
static Part enemyPart = {.previous = 0};
static Layer* indicator;

static char allyLevelBuffer[5];
static char enemyLevelBuffer[5];

DarkValue dark;

static void renderRect(Layer* layer, GContext* ctx, int alignment, GColor8 color, int percentage) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, COLOR_FALLBACK(color, GColorLightGray));
  int w = bounds.size.w * percentage / 100;
  int x = alignment == ALIGN_LEFT ? 0 : bounds.size.w - w;
  graphics_fill_rect(ctx, GRect(x, 0, w, bounds.size.h), 0, GCornerNone);
}

static void renderCircle(GContext* ctx, GColor8 color, int pos) {
  graphics_context_set_stroke_color(ctx, COLOR_FALLBACK(color, dark ? GColorWhite : GColorBlack));
  graphics_draw_round_rect(ctx, GRect(10 * pos, 0, 7, 4), 4);
}

static void renderBitmap(Part* part, ResourceValue resource) {
  if (part->previous == resource) {
    return;
  }
  part->previous = resource;
  gbitmap_destroy(part->bitmap);
  part->bitmap = helper_create_bitmap(resource, dark);
  bitmap_layer_set_bitmap(part->image, part->bitmap);
}

static GColor8 GColorFromHealth(int percentage) {
  return GColorFromRGB((100 - percentage) * 255 / 100, percentage * 255 / 100, 0);
}

static void renderAllyExperience(Layer* layer, GContext* ctx) {
  renderRect(layer, ctx, ALIGN_RIGHT, GColorBlue, state->ally->experience);
}

static void renderAllyHealth(Layer* layer, GContext* ctx) {
  renderRect(layer, ctx, ALIGN_LEFT, GColorFromHealth(state->ally->health), state->ally->health);
}

static void renderEnemyHealth(Layer* layer, GContext* ctx) {
  renderRect(layer, ctx, ALIGN_LEFT, GColorFromHealth(state->enemy->health), state->enemy->health);
}

static void renderEnemyExperience(Layer* layer, GContext* ctx) {
  renderRect(layer, ctx, ALIGN_LEFT, GColorVeryLightBlue, 100 * state->enemy->index_count / ENEMY_COUNT);
}

static void renderIndicator(Layer* layer, GContext* ctx) {
  graphics_context_set_antialiased(ctx, false);
  if (state->event & EVENT_SLEEP) {
    renderCircle(ctx, GColorSunsetOrange, 0);
  }
  if (state->event & (EVENT_EGG | EVENT_BOSS)) {
    renderCircle(ctx, dark ? GColorJaegerGreen : GColorMidnightGreen, 1);
  }
  if (state->event & (EVENT_GHOST | EVENT_BOSS)) {
    renderCircle(ctx, dark ? GColorPictonBlue : GColorBlueMoon, 2);
  }
  if (state->event & EVENT_MORPH) {
    renderCircle(ctx, GColorPurpureus, 3);
  }
  if (state->event & EVENT_EVO) {
    renderCircle(ctx, dark ? GColorChromeYellow : GColorLiberty, 4);
  }
}

void battlefield_load(Layer* root, State* stateRef) {
  state = stateRef;
  dark = state->settings & SETTINGS_DARK ? PBL_IF_BW_ELSE(DARK_FULL, DARK_ON) : DARK_OFF;

  allyPart.image = helper_create_bitmap_layer(root, GRect(10, 68, 48, 48), NULL);
  allyPart.level = helper_create_text_layer(root, GRect(72, 73, 25, 14), FONT_SMALL_BOLD, GTextAlignmentCenter, dark);
  allyPart.health = helper_create_layer(root, GRect(96, 91, 37, 4));
  allyPart.experience = helper_create_layer(root, GRect(76, 111, 61, 2));

  enemyPart.image = helper_create_bitmap_layer(root, GRect(82, 14, 56, 56), NULL);
  enemyPart.level = helper_create_text_layer(root, GRect(16, 10, 25, 14), FONT_SMALL_BOLD, GTextAlignmentCenter, dark);
  enemyPart.health = helper_create_layer(root, GRect(40, 28, 29, 3));
  enemyPart.experience = helper_create_layer(root, GRect(20, 41, 49, 1));

  indicator = helper_create_layer(root, GRect(21, 35, 47, 4));

  layer_set_update_proc(allyPart.health, renderAllyHealth);
  layer_set_update_proc(allyPart.experience, renderAllyExperience);
  layer_set_update_proc(enemyPart.health, renderEnemyHealth);
  layer_set_update_proc(enemyPart.experience, renderEnemyExperience);
  layer_set_update_proc(indicator, renderIndicator);
}

void battlefield_mark_dirty() {
  if (state->settings & SETTINGS_CACHE) {
    cache_layer_mark_dirty();
  }

  ResourceValue type = state->ally->type;
  if (state->quiet > QUIET_NONE) {
    type += 20;
  }
  if (state->ally->shiny) {
    type += 10;
  }
  renderBitmap(&allyPart, type);
  renderBitmap(&enemyPart, state->missing ? RESOURCE_ID_0 : state->enemy->type);

  int level = state->ally->level_final();
  snprintf(allyLevelBuffer, 5, "L%d", level > 100 ? level - 100 : level);
  text_layer_set_text(allyPart.level, allyLevelBuffer);

  snprintf(enemyLevelBuffer, 5, "L%d", state->enemy->level_final());
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
