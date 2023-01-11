#include <pebble.h>
#include "game.h"

static int calcLevelFromSteps(int steps) {
  int level = 0;
  int modifier = 15;
  while ((steps -= 5000) >= 0) {
    level += modifier;
    modifier -= modifier / 4;
  }
  level += modifier * (steps + 5000) / 5000;
  return level;
}

void game_level(State* state) {
  int level, experience;
  level = calcLevelFromSteps(state->health->steps);
  int stepsYesterday = state->health->steps_yesterday ?: 5000;
  experience = 100 * state->health->steps / stepsYesterday;
  if (experience > 100) {
    level += calcLevelFromSteps(state->health->steps - stepsYesterday);
    experience = 100;
  }
  state->ally->level = level < 1 ? 1 : level;
  state->ally->experience = experience;
  level = 30 * (state->health->sleep + state->health->restful_sleep) / 43200;
  state->enemy->level = level < 1 ? 1 : level;
}
