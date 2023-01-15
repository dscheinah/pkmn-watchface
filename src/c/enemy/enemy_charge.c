#include <pebble.h>
#include "enemy.h"
#include "helper.h"

bool enemy_charge(State* state) {
  switch (state->enemy->type) {
    case RESOURCE_ID_egg:
      if (state->health->steps <= 10000) {
        helper_evolve(state, RESOURCE_ID_25, 1, true);
        return true;
      }
      break;
    case RESOURCE_ID_132:
      helper_evolve(state, RESOURCE_ID_125, state->enemy->level_multiplier, false);
      return true;
    case RESOURCE_ID_133:
      helper_evolve(state, RESOURCE_ID_135, 2, false);
      return true;
    case RESOURCE_ID_25:
      if (state->enemy->health < 50) {
        helper_evolve(state, RESOURCE_ID_26, 2, false);
        state->enemy->health = 100;
        return true;
      }
      state->enemy->health = 100;
      break;
    case RESOURCE_ID_26:
      state->enemy->health = 100;
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
      if (state->enemy->health > 50) {
        state->enemy->health = 50;
      }
      break;
  }
  return false;
}
