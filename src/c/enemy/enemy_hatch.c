#include <pebble.h>
#include "enemy.h"
#include "helper.h"
#include "../state/const.h"

bool enemy_hatch(Enemy* enemy, Health health) {
  if (enemy->type != RESOURCE_ID_egg) {
    return false;
  }
  if (health.steps_yesterday > 24000) {
    helper_evolve(enemy, RESOURCE_ID_251, 4, true);
    return true;
  }
  if (health.steps_yesterday > 18000) {
    helper_evolve(enemy, RESOURCE_ID_151, 4, true);
    return true;
  }
  if (health.steps_yesterday > 10000) {
    helper_evolve(enemy, rand() % 3 + ENEMY_OFFSET, 1, true);
    return true;
  }
  helper_evolve(enemy, RESOURCE_ID_133, 1, true);
  return true;
}

bool enemy_hatch_bird(Enemy* enemy, Ally* ally) {
  if (enemy->type != RESOURCE_ID_egg) {
    return false;
  }
  if (helper_bird(enemy, ally) != RESOURCE_ID_250) {
    return false;
  }
  helper_evolve(enemy, RESOURCE_ID_250, 3, true);
  return true;
}
