#include <pebble.h>
#include "enemy.h"
#include "helper.h"

bool enemy_reset(State* state) {
  if (state->enemy->health > 0) {
    return false;
  }
  if (state->event & EVENT_BOSS) {
    helper_evolve(state, RESOURCE_ID_150, 3, true);
    return true;
  }
  if (state->event & EVENT_EGG) {
    state->enemy->type = RESOURCE_ID_egg;
    state->enemy->health = 100;
    state->enemy->level_multiplier = 1;
    state->enemy->hours_alive = 0;
    return true;
  }
  if (state->event & EVENT_GHOST) {
    helper_evolve(state, RESOURCE_ID_92, 1, true);
    return true;
  }
  if (state->event) {
    helper_evolve(state, helper_morph(state, false), 1, true);
    return true;
  }
  if (state->enemy->hours_alive < 3) {
    switch (state->enemy->type) {
      case RESOURCE_ID_92:
      case RESOURCE_ID_93:
        helper_evolve(state, state->enemy->type + 1, state->enemy->level_multiplier + 1, true);
        return true;
    }
  }
  if (state->enemy->type != RESOURCE_ID_143 && state->enemy->level >= 35) {
    helper_evolve(state, RESOURCE_ID_143, 1, true);
    return true;
  }
  if (state->enemy->hours_alive >= 8 && state->enemy->level_multiplier < 3) {
    helper_evolve(state, RESOURCE_ID_86, 1, true);
    return true;
  }
  if (state->counter > 512) {
    state->counter = 0;
    helper_evolve(state, RESOURCE_ID_185, 1, true);
    return true;
  }
  helper_evolve(state, RESOURCE_ID_133, 1, true);
  return true;
}

bool enemy_reset_bird(State* state) {
  if (state->enemy->health > 0 || state->enemy->type == RESOURCE_ID_150) {
    return false;
  }
  const int bird = helper_bird(state);
  if (!bird || bird == RESOURCE_ID_250) {
    return false;
  }
  helper_evolve(state, bird, bird == RESOURCE_ID_249 ? 3 : 2, true);
  return true;
}
