#include <pebble.h>
#include "enemy.h"
#include "helper.h"

bool enemy_evolution(State* state) {
  if (state->event & EVENT_MORPH) {
    helper_evolve(state, helper_morph(state, true), state->enemy->level_multiplier, false);
    return true;
  }
  if (state->event & EVENT_SLEEP) {
    switch (state->enemy->type) {
      case RESOURCE_ID_egg:
        if (state->health->steps <= 10000) {
          helper_evolve(state, RESOURCE_ID_39, 1, true);
          return true;
        }
        break;
      case RESOURCE_ID_132:
        helper_evolve(state, RESOURCE_ID_200, state->enemy->level_multiplier, false);
        return true;
    }
    return false;
  }
  if (state->event & EVENT_EVO) {
    switch (state->enemy->type) {
      case RESOURCE_ID_133:
        if ((rand() % 2 + 3) - state->enemy->hours_alive <= 0) {
          helper_evolve(state, RESOURCE_ID_196, 2, false);
        } else {
          helper_evolve(state, RESOURCE_ID_134 + rand() % 2, 2, false);
        }
        return true;
      case RESOURCE_ID_86:
        helper_evolve(state, RESOURCE_ID_87, 2, false);
        return true;
      case RESOURCE_ID_216:
        helper_evolve(state, RESOURCE_ID_217, 2, false);
        return true;
      default:
        if (state->enemy->type >= RESOURCE_ID_1 && state->enemy->type <= RESOURCE_ID_8) {
          helper_evolve(state, state->enemy->type + 3, state->enemy->level_multiplier + 1, false);
          state->enemy->hours_alive = 0;
          return true;
        }
    }
  }
  state->enemy->hours_alive++;
  return false;
}

bool enemy_evolution_night(State* state) {
  switch (state->enemy->type) {
    case RESOURCE_ID_133:
      helper_evolve(state, RESOURCE_ID_197, 2, false);
      return true;
    case RESOURCE_ID_92:
    case RESOURCE_ID_93:
      helper_evolve(state, state->enemy->type + 1, state->enemy->level_multiplier + 1, false);
      state->enemy->hours_alive = 0;
      return true;
  }
  return false;
}
