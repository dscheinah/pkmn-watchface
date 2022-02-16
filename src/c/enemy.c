#include <pebble.h>
#include "enemy.h"
#define VERSION 1
#define VERSION_KEY 200
#define TYPE_KEY 201
#define LEVEL_KEY 202
#define HEALTH_KEY 203
#define ALIVE_KEY 204

static Enemy enemy;

static int level_final() {
  int level = enemy.level * enemy.level_multiplier;
  return level > 999 ? 999 : level;
}

Enemy* enemy_init() {
  enemy.level = 1;
  if (persist_exists(VERSION_KEY) && persist_read_int(VERSION_KEY) == VERSION) {
    enemy.type = persist_read_int(TYPE_KEY);
    enemy.level_multiplier = persist_read_int(LEVEL_KEY);
    enemy.health = persist_read_int(HEALTH_KEY);
    enemy.hours_alive = persist_read_int(ALIVE_KEY);
  } else {
    enemy.type = RESOURCE_ID_133;
    enemy.level_multiplier = 1;
    enemy.health = 100;
    enemy.hours_alive = 0;
  }
  enemy.level_final = &level_final;
  return &enemy;
}

bool enemy_reset(bool egg, bool ghost) {
  if (enemy.health > 0) {
    return false;
  }
  enemy.health = 100;
  if (egg) {
    enemy.type = RESOURCE_ID_egg;
    enemy.level_multiplier = 1;
    enemy.hours_alive = 0;
    return true;
  }
  if (ghost && enemy.level_multiplier > 1) {
    enemy.type = RESOURCE_ID_92;
    enemy.level_multiplier = 1;
    enemy.hours_alive = 0;
    return true;
  }
  if (enemy.hours_alive > 10) {
    enemy.type = RESOURCE_ID_86;
    enemy.level_multiplier = 1;
    enemy.hours_alive = 0;
    return true;
  }
  if (enemy.hours_alive < 3 && enemy.type == RESOURCE_ID_92) {
    enemy.type = RESOURCE_ID_93;
    enemy.level_multiplier = 2;
    enemy.hours_alive = 0;
    return true;
  }
  enemy.level_multiplier = 1;
  enemy.hours_alive = 0;
  if (enemy.type == RESOURCE_ID_133) {
    enemy.type = RESOURCE_ID_25;
    return true;
  }
  enemy.type = RESOURCE_ID_133;
  return true;
}

bool enemy_evolution(Health health) {
  if (health.restful_sleep_hour) {
    return false;
  }
  enemy.hours_alive++;
  if (rand() % 5 >= enemy.hours_alive) {
    return false;
  }
  switch (enemy.type) {
    case RESOURCE_ID_133:
      enemy.type = rand() % 3 + 28;
      enemy.level_multiplier = 2;
      enemy.hours_alive = 0;
      return true;
    case RESOURCE_ID_86:
      enemy.type = RESOURCE_ID_87;
      enemy.level_multiplier = 2;
      enemy.hours_alive = 0;
      return true;
    default:
      if (enemy.type >= 14 && enemy.type <= 19) {
        enemy.type += 3;
        enemy.level_multiplier++;
        enemy.hours_alive = 0;
        return true;
      }
  }
  return false;
}

bool enemy_night() {
  switch (enemy.type) {
    case RESOURCE_ID_133:
      enemy.type = RESOURCE_ID_197;
      enemy.level_multiplier = 2;
      enemy.hours_alive = 0;
      return true;
    case RESOURCE_ID_93:
      enemy.type = RESOURCE_ID_94;
      enemy.level_multiplier = 3;
      enemy.hours_alive = 0;
      return true;
  }
  return false;
}

bool enemy_charge() {
  switch (enemy.type) {
    case RESOURCE_ID_133:
      enemy.type = RESOURCE_ID_135;
      enemy.level_multiplier = 2;
      enemy.hours_alive = 0;
      return true;
    case RESOURCE_ID_25:
      enemy.type = RESOURCE_ID_26;
      enemy.level_multiplier = 2;
      enemy.health = 100;
      enemy.hours_alive = 0;
      return true;
    case RESOURCE_ID_26:
      if (enemy.health < 100) {
        enemy.health = 100;
      }
      break;
    case RESOURCE_ID_7:
    case RESOURCE_ID_8:
    case RESOURCE_ID_9:
    case RESOURCE_ID_134:
      if (enemy.health > 50) {
        enemy.health = 50;
      }
      break;
  }
  return false;
}

bool enemy_hatch(Health health) {
  if (enemy.type != RESOURCE_ID_egg) {
    return false;
  }
  enemy.health = 100;
  enemy.hours_alive = 0;
  if (health.steps_yesterday > 20000) {
    enemy.type = RESOURCE_ID_151;
    enemy.level_multiplier = 4;
    return true;
  }
  enemy.level_multiplier = 1;
  if (health.steps_yesterday > 10000) {
    enemy.type = rand() % 3 + 14;
    return true;
  }
  enemy.type = RESOURCE_ID_133;
  return true;
}

void enemy_deinit() {
  persist_write_int(VERSION_KEY, VERSION);
  persist_write_int(TYPE_KEY, enemy.type);
  persist_write_int(LEVEL_KEY, enemy.level_multiplier);
  persist_write_int(HEALTH_KEY, enemy.health);
  persist_write_int(ALIVE_KEY, enemy.hours_alive);
}
