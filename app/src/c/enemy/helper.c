#include <pebble.h>
#include "helper.h"

void helper_evolve(State* state, ResourceValue type, int level, bool reset) {
  state->enemy->type = type;
  state->enemy->level_multiplier = level;
  if (reset) {
    state->enemy->health = 100;
    state->enemy->morph = type != RESOURCE_ID_150 && rand() % 8 == 0;
    state->enemy->hours_alive = 0;
  }
}

ResourceValue helper_bird(State* state) {
  int bird144 = 0;
  switch (state->enemy->type) {
    case RESOURCE_ID_egg:
    case RESOURCE_ID_86:
    case RESOURCE_ID_87:
      bird144 = state->enemy->hours_alive;
  }
  if (rand() % 100 < bird144) {
    return RESOURCE_ID_144;
  }

  int bird145 = state->enemy->type != RESOURCE_ID_145 && state->ally->health >= 50 ? (state->ally->health - 45) / 5 : 0;
  if (rand() % 100 < bird145) {
    return RESOURCE_ID_145;
  }

  int bird146 = state->enemy->type != RESOURCE_ID_146 && state->ally->level >= 20 ? (state->ally->level - 14) / 6 : 0;
  if (rand() % 100 < bird146) {
    return RESOURCE_ID_146;
  }

  int bird249 = state->enemy->type != RESOURCE_ID_249 && state->enemy->level >= 25 ? (state->enemy->level - 24) / 4 : 0;
  if (rand() % 100 < bird249) {
    return RESOURCE_ID_249;
  }

  if (rand() % 100 < (bird144 + bird145 + bird146 + bird249) / 7) {
    return RESOURCE_ID_250;
  }
  return 0;
}
