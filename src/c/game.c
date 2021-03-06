#include <pebble.h>
#include "game.h"

static float levelModifier(float diff, int type) {
  float level = 1.0 + (diff / 100);
  if (type > 3) {
    level += 0.15;
  }
  if (type > 6) {
    level += 0.15;
  }
  if (level < 0.25) {
    return 0.25;
  }
  if (level > 3) {
    return 3;
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
        case 1:
          return 0.5;
        case 2:
          return 2;
      }
      break;
    case RESOURCE_ID_2:
    case RESOURCE_ID_5:
    case RESOURCE_ID_8:
    case RESOURCE_ID_136:
    case RESOURCE_ID_146:
    case RESOURCE_ID_250:
      switch (ally->type % 3) {
        case 0:
          return 2;
        case 1:
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
        case 0:
          return 0.5;
        case 1:
        case 2:
          return 2;
      }
      break;
    case RESOURCE_ID_92:
    case RESOURCE_ID_93:
    case RESOURCE_ID_94:
    case RESOURCE_ID_145:
    case RESOURCE_ID_249:
      if (ally->type % 3 == 1) {
        return 0.5;
      }
      break;
    case RESOURCE_ID_213:
      if (ally->type % 3 == 0) {
        return 2;
      }
      break;
    case RESOURCE_ID_144:
    case RESOURCE_ID_214:
    case RESOURCE_ID_225:
      switch (ally->type % 3) {
        case 1:
          return 0.5;
        case 2:
          return 2;
      }
      break;
    case RESOURCE_ID_251:
      if (ally->type % 3 == 2) {
        return 2;
      }
      break;
  }
  return 1;
}

void game_set_ally_level(Ally *ally, Health health) {
  int level = 30 * health.steps / 10000;
  int experience = 100 * health.steps / (health.steps_yesterday ?: 5000);
  if (experience > 100) {
    level += 30 * (health.steps - health.steps_yesterday) / 10000;
    experience = 100;
  }
  ally->level = level < 1 ? 1 : level;
  ally->experience = experience;
}

void game_set_enemy_level(Enemy *enemy, Health health) {
  int level = 30 * (health.sleep + health.restful_sleep) / 47520;
  enemy->level = level < 1 ? 1 : level;
}

bool game_deal_damage(Ally *ally, Enemy *enemy, Health health) {
  if (health.restful_sleep_hour || enemy->type == RESOURCE_ID_egg) {
    return false;
  }
  float level = levelModifier(ally->level_final() - enemy->level_final(), ally->type);
  float effective = effectiveModifier(ally, enemy);
  enemy->health -= (90.0 * health.active_hour / 3600 + 10) * level * effective;
  return true;
}
