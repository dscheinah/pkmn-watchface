#include <pebble.h>
#include "enemy.h"
#include "helper.h"
#include "../state/const.h"

bool enemy_reset(Enemy* enemy, EventValue event) {
  if (enemy->health > 0) {
    return false;
  }
  if (enemy->type == RESOURCE_ID_150) {
    enemy->index_count = 0;
    enemy->index[0] = 0;
    enemy->index[1] = 0;
  }
  if (event & EVENT_BOSS) {
    helper_evolve(enemy, RESOURCE_ID_150, 3, true);
    return true;
  }
  if (event & EVENT_EGG) {
    enemy->type = RESOURCE_ID_egg;
    enemy->health = 100;
    enemy->level_multiplier = 1;
    enemy->hours_alive = 0;
    return true;
  }
  if (event & EVENT_GHOST) {
    helper_evolve(enemy, RESOURCE_ID_92, 1, true);
    return true;
  }
  if (enemy->hours_alive > 9) {
    helper_evolve(enemy, RESOURCE_ID_86, 1, true);
    return true;
  }
  if (enemy->hours_alive <= 3 && (enemy->type == RESOURCE_ID_92 || enemy->type == RESOURCE_ID_93)) {
    helper_evolve(enemy, enemy->type + 1, enemy->level_multiplier + 1, true);
    return true;
  }
  helper_evolve(enemy, enemy->type == RESOURCE_ID_133 ? RESOURCE_ID_25 : RESOURCE_ID_133, 1, true);
  return true;
}

bool enemy_reset_bird(Enemy* enemy, Ally *ally) {
  if (enemy->health > 0 || enemy->type == RESOURCE_ID_150) {
    return false;
  }
  int bird = helper_bird(enemy, ally);
  if (!bird || bird == RESOURCE_ID_250) {
    return false;
  }
  helper_evolve(enemy, bird, bird == RESOURCE_ID_249 ? 3 : 2, true);
  return true;
}
