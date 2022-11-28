#include <pebble.h>
#include "enemy.h"
#include "const.h"
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

static Enemy enemy = {.level = 1};

static int level_final() {
  int level = enemy.level * enemy.level_multiplier;
  if (level < 1) {
    return 1;
  }
  return level > 999 ? 999 : level;
}

static void evolve(int type, int level, bool reset) {
  enemy.type = type;
  enemy.level_multiplier = level;
  int key = 0, chk = type - 23;
  if (chk > 31) {
    key = 1;
    chk -= 32;
  }
  int pos = 1 << chk;
  if (!(enemy.index[key] & pos)) {
    enemy.index[key] = enemy.index[key] | pos;
    enemy.index_count++;
  }
  if (reset) {
    enemy.health = 100;
    enemy.morph = rand() % 8 == 0;
    enemy.hours_alive = 0;
  }
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
    evolve(RESOURCE_ID_133, 1, true);
  }
  enemy.level_final = &level_final;
  return &enemy;
}

bool enemy_reset(EventValue event) {
  if (enemy.health > 0) {
    return false;
  }
  if (enemy.type == RESOURCE_ID_150) {
    enemy.index_count = 0;
    enemy.index[0] = 0;
    enemy.index[1] = 0;
  } else if (event & EVENT_BOSS) {
    evolve(RESOURCE_ID_150, 4, true);
    return true;
  }
  if (event & EVENT_EGG) {
    enemy.type = RESOURCE_ID_egg;
    enemy.health = 100;
    enemy.level_multiplier = 1;
    enemy.hours_alive = 0;
    return true;
  }
  if (event & EVENT_GHOST) {
    evolve(RESOURCE_ID_92, 1, true);
    return true;
  }
  if (enemy.hours_alive > 9) {
    evolve(RESOURCE_ID_86, 1, true);
    return true;
  }
  if (enemy.hours_alive <= 3 && (enemy.type == RESOURCE_ID_92 || enemy.type == RESOURCE_ID_93)) {
    evolve(enemy.type + 1, enemy.level_multiplier + 1, true);
    return true;
  }
  evolve(enemy.type == RESOURCE_ID_133 ? RESOURCE_ID_25 : RESOURCE_ID_133, 1, true);
  return true;
}

bool enemy_evolution(EventValue event) {
  if (event & EVENT_MORPH) {
    evolve(enemy.type == RESOURCE_ID_132 ? rand() % 6 + 47 : RESOURCE_ID_132, enemy.level_multiplier, false);
    return true;
  }
  if (event & EVENT_SLEEP) {
    return false;
  }
  if (event & EVENT_EVO) {
    switch (enemy.type) {
      case RESOURCE_ID_133:
        evolve(enemy.type + 1 + rand() % 3, 2, false);
        return true;
      case RESOURCE_ID_86:
        evolve(RESOURCE_ID_87, 2, false);
        return true;
      default:
        if (enemy.type >= 23 && enemy.type <= 28) {
          evolve(enemy.type + 3, enemy.level_multiplier + 1, false);
          enemy.hours_alive = 0;
          return true;
        }
    }
  }
  enemy.hours_alive++;
  return false;
}

bool enemy_night() {
  switch (enemy.type) {
    case RESOURCE_ID_133:
      evolve(RESOURCE_ID_197, 2, false);
      return true;
    case RESOURCE_ID_92:
    case RESOURCE_ID_93:
      evolve(enemy.type + 1, enemy.level_multiplier + 1, false);
      enemy.hours_alive = 0;
      return true;
  }
  return false;
}

bool enemy_charge() {
  switch (enemy.type) {
    case RESOURCE_ID_133:
      evolve(RESOURCE_ID_135, 2, false);
      return true;
    case RESOURCE_ID_25:
      evolve(RESOURCE_ID_26, 2, false);
      enemy.health = 100;
      return true;
    case RESOURCE_ID_26:
      enemy.health = 100;
      break;
    case RESOURCE_ID_7:
    case RESOURCE_ID_8:
    case RESOURCE_ID_9:
    case RESOURCE_ID_86:
    case RESOURCE_ID_87:
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
  if (health.steps_yesterday > 24000) {
    evolve(RESOURCE_ID_251, 4, true);
    return true;
  }
  if (health.steps_yesterday > 18000) {
    evolve(RESOURCE_ID_151, 4, true);
    return true;
  }
  if (health.steps_yesterday > 10000) {
    evolve(rand() % 3 + 23, 1, true);
    return true;
  }
  evolve(RESOURCE_ID_133, 1, true);
  return true;
}

bool enemy_quiet(bool quiet, Health health) {
  if (quiet) {
    switch (enemy.type) {
      case RESOURCE_ID_egg:
        if (health.steps <= 10000) {
          evolve(RESOURCE_ID_175, 1, true);
          return true;
        }
        break;
      case RESOURCE_ID_25:
        evolve(RESOURCE_ID_172, 0, false);
        return true;
    }
  } else if (enemy.type == RESOURCE_ID_175) {
    evolve(RESOURCE_ID_176, 2, false);
    return true;
  }
  return false;
}

static int checkBird(Ally *ally) {
  int bird144 = enemy.hours_alive * (enemy.type == RESOURCE_ID_86 || enemy.type == RESOURCE_ID_87 ? 20 : 1) / 4;
  int bird145 = ally->health / 15;
  int bird146 = ally->level / 10;
  int bird249 = enemy.level / 10;
  if (rand() % 100 < bird144) {
    return RESOURCE_ID_144;
  }
  if (rand() % 100 < bird145) {
    return RESOURCE_ID_145;
  }
  if (rand() % 100 < bird146) {
    return RESOURCE_ID_146;
  }
  if (rand() % 100 < bird249) {
    return RESOURCE_ID_249;
  }
  if (rand() % 100 < (bird144 + bird145 + bird146 + bird249) / 4) {
    return RESOURCE_ID_250;
  }
  return 0;
}

bool enemy_bird(Ally *ally, bool reset) {
  int bird = checkBird(ally);
  if (!bird) {
    return false;
  }
  if (reset) {
    if (bird == RESOURCE_ID_250 || enemy.health > 0) {
      return false;
    }
    evolve(bird, 3, true);
    return true;
  }
  if (bird != RESOURCE_ID_250 || enemy.type != RESOURCE_ID_egg) {
    return false;
  }
  evolve(bird, 3, true);
  return true;
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
