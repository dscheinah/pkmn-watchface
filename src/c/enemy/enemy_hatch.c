#include <pebble.h>
#include "enemy.h"
#include "helper.h"

bool enemy_hatch(State* state) {
  if (state->enemy->type != RESOURCE_ID_egg) {
    return false;
  }
  if (state->health->steps_yesterday > 24000) {
    helper_evolve(state, RESOURCE_ID_251, 4, true);
    return true;
  }
  if (state->health->steps_yesterday > 18000) {
    helper_evolve(state, RESOURCE_ID_151, 4, true);
    return true;
  }
  if (state->health->steps_yesterday > 10000) {
    helper_evolve(state, rand() % 3 + ENEMY_OFFSET, 1, true);
    return true;
  }
  helper_evolve(state, RESOURCE_ID_133, 1, true);
  return true;
}

bool enemy_hatch_bird(State* state) {
  if (state->enemy->type != RESOURCE_ID_egg) {
    return false;
  }
  if (helper_bird(state) != RESOURCE_ID_250) {
    return false;
  }
  helper_evolve(state, RESOURCE_ID_250, 3, true);
  return true;
}
