#include <pebble.h>
#include "enemy.h"
#include "helper.h"
#include "../state/const.h"

bool enemy_evolution(Enemy* enemy, EventValue event) {
  if (event & EVENT_MORPH) {
    if (enemy->type == RESOURCE_ID_132) {
      helper_evolve(enemy, rand() % 6 + ENEMY_OFFSET + 24, enemy->level_multiplier, false);
    } else {
      helper_evolve(enemy, RESOURCE_ID_132, enemy->level_multiplier, false);
    }
    return true;
  }
  if (event & EVENT_SLEEP) {
    return false;
  }
  if (event & EVENT_EVO) {
    switch (enemy->type) {
      case RESOURCE_ID_133:
        if (enemy->hours_alive > 4 || (enemy->hours_alive > 3 && rand() % 2 == 0)) {
          helper_evolve(enemy, RESOURCE_ID_196, 2, false);
        } else {
          helper_evolve(enemy, enemy->type + 1 + rand() % 2, 2, false);
        }
        return true;
      case RESOURCE_ID_86:
        helper_evolve(enemy, RESOURCE_ID_87, 2, false);
        return true;
      default:
        if (enemy->type >= ENEMY_OFFSET && enemy->type <= ENEMY_OFFSET + 5) {
          helper_evolve(enemy, enemy->type + 3, enemy->level_multiplier + 1, false);
          enemy->hours_alive = 0;
          return true;
        }
    }
  }
  enemy->hours_alive++;
  return false;
}

bool enemy_evolution_night(Enemy* enemy) {
  switch (enemy->type) {
    case RESOURCE_ID_133:
      helper_evolve(enemy, RESOURCE_ID_197, 2, false);
      return true;
    case RESOURCE_ID_92:
    case RESOURCE_ID_93:
      helper_evolve(enemy, enemy->type + 1, enemy->level_multiplier + 1, false);
      enemy->hours_alive = 0;
      return true;
  }
  return false;
}
