#include <pebble.h>
#include "enemy.h"
#include "helper.h"

bool enemy_quiet_enable(Enemy* enemy, Health health) {
  switch (enemy->type) {
    case RESOURCE_ID_egg:
      if (health.steps <= 10000) {
        helper_evolve(enemy, RESOURCE_ID_175, 1, true);
        return true;
      }
      break;
    case RESOURCE_ID_25:
      helper_evolve(enemy, RESOURCE_ID_172, 0, false);
      return true;
  }
  return false;
}

bool enemy_quiet_disable(Enemy* enemy) {
  if (enemy->type == RESOURCE_ID_175) {
    helper_evolve(enemy, RESOURCE_ID_176, 2, false);
    return true;
  }
  return false;
}
