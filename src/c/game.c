#include <pebble.h>
#include "game.h"

static float levelModifier(float diff) {
  float level = 1.0 + (diff / 100);
  if (level < 0.25) {
    return 0.25;
  }
  if (level > 2) {
    return 2;
  }
  return level;
}

static float effectiveModifier(Ally *ally, Enemy *enemy) {
  switch (enemy->type) {
    case RESOURCE_ID_1:
    case RESOURCE_ID_4:
    case RESOURCE_ID_7:
      switch (ally->type % 3) {
        case 0:
          return 0.5;
        case 1:
          return 0.25;
        case 2:
          return 2;
      }
      break;
    case RESOURCE_ID_2:
    case RESOURCE_ID_5:
    case RESOURCE_ID_8:
    case RESOURCE_ID_136:
      switch (ally->type % 3) {
        case 0:
          return 2;
        case 1:
          return 0.5;
        case 2:
          return 0.5;
      }
      break;
    case RESOURCE_ID_3:
    case RESOURCE_ID_6:
    case RESOURCE_ID_9:
    case RESOURCE_ID_134:
      switch (ally->type % 3) {
        case 1:
          return 2;
        case 2:
          return 0.5;
      }
      break;
    case RESOURCE_ID_86:
    case RESOURCE_ID_87:
      switch (ally->type % 3) {
        case 2:
          return 2;
      }
      break;
  }
  return 1;
}

int game_set_ally_level(Ally *ally, Health health) {
  float level = 30.0 * health.steps / 10000;
  float experience = (float) health.steps / (health.steps_yesterday ?: 5000);
  if (level < 1) {
    level = 1;
  }
  if (experience > 1) {
    level += 30.0 * (health.steps - health.steps_yesterday) / 10000;
    experience = 1;
  }
  if (ally->level != level || ally->experience != experience) {
    ally->level = level;
    ally->experience = experience;
    return 1;
  }
  return 0;
}

int game_set_enemy_level(Enemy *enemy, Health health) {
  float level = 30.0 * (health.sleep + health.restful_sleep) / 47520;
  if (level < 1) {
    level = 1;
  }
  if (level != enemy->level) {
    enemy->level = level;
    return 1;
  }
  return 0;
}

int game_deal_damage(Ally *ally, Enemy *enemy, Health health) {
  if (health.restful_sleep_hour || enemy->type == RESOURCE_ID_egg) {
    return 0;
  }
  float level = levelModifier(ally->level_final() - enemy->level_final());
  float effective = effectiveModifier(ally, enemy);
  enemy->health -= (90.0 * health.active_hour / 3600 + 10) * level * effective;
  return 1;
}
