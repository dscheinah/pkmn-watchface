#include <pebble.h>
#include "game.h"

static int calcLevelFromSteps(int steps) {
  int level = 0;
  int modifier = 26;
  while ((steps -= 9000) >= 0) {
    level += modifier;
    modifier -= modifier / 3;
  }
  level += modifier * (steps + 9000) / 9000;
  return level;
}

void game_level(State* state) {
  int level, experience;
  level = calcLevelFromSteps(state->health->steps);
  experience = 100 * state->health->steps / (state->health->steps_yesterday ?: 5000);
  if (experience > 100) {
    level += calcLevelFromSteps(state->health->steps - state->health->steps_yesterday);
    experience = 100;
  }
  state->ally->level = level < 1 ? 1 : level;
  state->ally->experience = experience;
  level = 30 * (state->health->sleep + state->health->restful_sleep) / 47520;
  state->enemy->level = level < 1 ? 1 : level;
}
