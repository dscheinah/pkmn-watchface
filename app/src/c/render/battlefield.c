#include <pebble.h>
#include "battlefield.h"
#include "cache.h"
#include "helper.h"
#include "monster.h"

static State* state;
static MonsterPart allyPart = {.previous = 0};
static MonsterPart enemyPart = {.previous = 0};
static Layer* indicator;

static Layer* allyExperience;
static Layer* enemyExperience;

static char allyLevelBuffer[5];
static char enemyLevelBuffer[5];

DarkValue dark;

static void renderCircle(GContext* ctx, GColor8 color, int pos) {
  graphics_context_set_stroke_color(ctx, COLOR_FALLBACK(color, dark ? GColorWhite : GColorBlack));
  graphics_draw_round_rect(ctx, GRect(10 * pos, 0, 7, 4), 4);
}

static void renderAllyExperience(Layer* layer, GContext* ctx) {
  helper_render_percentage_rect(layer, ctx, ALIGN_RIGHT, GColorBlue, state->ally->experience);
}

static void renderAllyHealth(Layer* layer, GContext* ctx) {
  GColor8 color = state->charging ? GColorLimerick : helper_color_from_health(state->ally->health);
  helper_render_percentage_rect(layer, ctx, ALIGN_LEFT, color, state->ally->health);
}

static void renderEnemyHealth(Layer* layer, GContext* ctx) {
  helper_render_percentage_rect(layer, ctx, ALIGN_LEFT, helper_color_from_health(state->enemy->health), state->enemy->health);
}

static void renderEnemyExperience(Layer* layer, GContext* ctx) {
  helper_render_percentage_rect(layer, ctx, ALIGN_LEFT, GColorVeryLightBlue, 100 * state->enemy->index_count / ENEMY_COUNT);
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

  monster_load_ally(root, &allyPart, dark);
  allyExperience = helper_create_layer(root, GRect(76, 111, 61, 2));

  monster_load_enemy(root, &enemyPart, dark);
  enemyExperience = helper_create_layer(root, GRect(20, 41, 49, 1));

  indicator = helper_create_layer(root, GRect(21, 35, 47, 4));

  layer_set_update_proc(allyPart.health, renderAllyHealth);
  layer_set_update_proc(allyExperience, renderAllyExperience);
  layer_set_update_proc(enemyPart.health, renderEnemyHealth);
  layer_set_update_proc(enemyExperience, renderEnemyExperience);
  layer_set_update_proc(indicator, renderIndicator);
}

void battlefield_mark_dirty() {
  if (state->settings & SETTINGS_CACHE) {
    cache_layer_mark_dirty();
  }

  ResourceValue type = state->ally->type;
  if (state->quiet > QUIET_NONE || quiet_time_is_active()) {
    type += type == RESOURCE_ID_a201x ? 2 : 20;
  }
  if (state->ally->shiny) {
    type += type == RESOURCE_ID_a201x ? 1 : 10;
  }
  monster_render(&allyPart, type, dark);
  monster_render(&enemyPart, state->missing ? RESOURCE_ID_0 : state->enemy->type, dark);

  int level = state->ally->level_final();
  snprintf(allyLevelBuffer, 5, "L%d", level > 100 ? level - 100 : level);
  text_layer_set_text(allyPart.level, allyLevelBuffer);

  snprintf(enemyLevelBuffer, 5, "L%d", state->enemy->level_final());
  text_layer_set_text(enemyPart.level, enemyLevelBuffer);
}

void battlefield_unload() {
  gbitmap_destroy(allyPart.bitmap);
  gbitmap_destroy(enemyPart.bitmap);

  monster_unload(&allyPart);
  layer_destroy(allyExperience);

  monster_unload(&enemyPart);
  layer_destroy(enemyExperience);

  layer_destroy(indicator);
}
