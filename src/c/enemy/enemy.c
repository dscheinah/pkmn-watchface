#include <pebble.h>
#include "enemy.h"
#include "helper.h"
#define VERSION 2
#define VERSION_KEY 200
#define TYPE_KEY 201
#define LEVEL_KEY 202
#define HEALTH_KEY 203
#define ALIVE_KEY 204
#define MORPH_KEY 205
#define INDEX_COUNT_KEY 206
#define INDEX_0_KEY 207
#define INDEX_1_KEY 208

static Enemy enemy = {.level = 1, .missing = false};

static int level_final() {
  int level = enemy.level * enemy.level_multiplier;
  if (level < 1) {
    return 1;
  }
  return level > 999 ? 999 : level;
}

Enemy* enemy_init() {
  if (persist_exists(VERSION_KEY) && persist_read_int(VERSION_KEY) == VERSION) {
    enemy.type = persist_read_int(TYPE_KEY);
    enemy.level_multiplier = persist_read_int(LEVEL_KEY);
    enemy.health = persist_read_int(HEALTH_KEY);
    enemy.hours_alive = persist_read_int(ALIVE_KEY);
    enemy.morph = persist_read_bool(MORPH_KEY);
    enemy.index_count = persist_read_int(INDEX_COUNT_KEY);
    enemy.index[0] = persist_read_int(INDEX_0_KEY);
    enemy.index[1] = persist_read_int(INDEX_1_KEY);
  } else {
    enemy.index_count = 0;
    enemy.index[0] = 0;
    enemy.index[1] = 0;
    helper_evolve(&enemy, RESOURCE_ID_133, 1, true);
  }
  enemy.level_final = &level_final;
  return &enemy;
}

void enemy_deinit() {
  persist_write_int(VERSION_KEY, VERSION);
  persist_write_int(TYPE_KEY, enemy.type);
  persist_write_int(LEVEL_KEY, enemy.level_multiplier);
  persist_write_int(HEALTH_KEY, enemy.health);
  persist_write_int(ALIVE_KEY, enemy.hours_alive);
  persist_write_bool(MORPH_KEY, enemy.morph);
  persist_write_int(INDEX_COUNT_KEY, enemy.index_count);
  persist_write_int(INDEX_0_KEY, enemy.index[0]);
  persist_write_int(INDEX_1_KEY, enemy.index[1]);
}
