#include <pebble.h>
#include "enemy.h"
#include "helper.h"

bool enemy_charge(Enemy* enemy) {
  switch (enemy->type) {
    case RESOURCE_ID_133:
      helper_evolve(enemy, RESOURCE_ID_135, 2, false);
      return true;
    case RESOURCE_ID_25:
      helper_evolve(enemy, RESOURCE_ID_26, 2, false);
      enemy->health = 100;
      return true;
    case RESOURCE_ID_26:
      enemy->health = 100;
      break;
    case RESOURCE_ID_7:
    case RESOURCE_ID_8:
    case RESOURCE_ID_9:
    case RESOURCE_ID_86:
    case RESOURCE_ID_87:
    case RESOURCE_ID_134:
    case RESOURCE_ID_144:
    case RESOURCE_ID_146:
    case RESOURCE_ID_176:
    case RESOURCE_ID_225:
    case RESOURCE_ID_249:
    case RESOURCE_ID_250:
      if (enemy->health > 50) {
        enemy->health = 50;
      }
      break;
  }
  return false;
}
