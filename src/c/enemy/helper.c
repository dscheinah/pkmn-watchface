#include <pebble.h>
#include "helper.h"
#include "../state/const.h"

void helper_evolve(Enemy* enemy, int type, int level, bool reset) {
  enemy->type = type;
  enemy->level_multiplier = level;
  int key = 0, chk = type - ENEMY_OFFSET;
  if (chk > 31) {
    key = 1;
    chk -= 32;
  }
  int pos = 1 << chk;
  if (!(enemy->index[key] & pos)) {
    enemy->index[key] = enemy->index[key] | pos;
    enemy->index_count++;
  }
  if (reset) {
    enemy->health = 100;
    enemy->morph = rand() % 8 == 0;
    enemy->hours_alive = 0;
  }
}

int helper_bird(Enemy* enemy, Ally* ally) {
  int bird144 = 0;
  switch (enemy->type) {
    case RESOURCE_ID_egg:
    case RESOURCE_ID_86:
    case RESOURCE_ID_87:
      bird144 = enemy->hours_alive;
  }
  if (rand() % 100 < bird144) {
    return RESOURCE_ID_144;
  }

  int bird145 = ally->health >= 50 ? (ally->health - 45) / 5 : 0;
  if (rand() % 100 < bird145) {
    return RESOURCE_ID_145;
  }

  int bird146 = ally->level >= 25 ? (ally->level - 18) / 7 : 0;
  if (rand() % 100 < bird146) {
    return RESOURCE_ID_146;
  }

  int bird249 = enemy->level >= 25 ? (enemy->level - 22) / 3 : 0;
  if (rand() % 100 < bird249) {
    return RESOURCE_ID_249;
  }

  if (rand() % 100 < (bird144 + bird145 + bird146 + bird249) / 8) {
    return RESOURCE_ID_250;
  }
  return 0;
}
