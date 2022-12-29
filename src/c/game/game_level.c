#include <pebble.h>
#include "game.h"

static int calcLevelFromSteps(int steps) {
  int level = 0;
  int modifier = 26;
  while ((steps -= 7500) >= 0) {
    level += modifier;
    modifier -= modifier / 3;
  }
  level += modifier * (steps + 7500) / 7500;
  return level;
}

void game_level_set_ally(Ally *ally, Health health) {
  int level = calcLevelFromSteps(health.steps);
  int experience = 100 * health.steps / (health.steps_yesterday ?: 5000);
  if (experience > 100) {
    level += calcLevelFromSteps(health.steps - health.steps_yesterday);
    experience = 100;
  }
  ally->level = level < 1 ? 1 : level;
  ally->experience = experience;
}

void game_level_set_enemy(Enemy *enemy, Health health) {
  int level = 30 * (health.sleep + health.restful_sleep) / 47520;
  enemy->level = level < 1 ? 1 : level;
}
