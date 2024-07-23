#include <pebble.h>
#include "game.h"
#define WEAK_PLANT 1
#define WEAK_FIRE 2
#define WEAK_WATER 4
#define WEAK_STONE 8
#define STRONG_PLANT 16
#define STRONG_FIRE 32
#define STRONG_WATER 64

static const uint16_t types[ENEMY_COUNT] = {
  /* 001 */ WEAK_FIRE | STRONG_PLANT | STRONG_WATER,
  /* 004 */ WEAK_WATER | WEAK_STONE | STRONG_PLANT | STRONG_FIRE,
  /* 007 */ WEAK_PLANT | STRONG_FIRE | STRONG_WATER,
  /* 002 */ WEAK_FIRE | STRONG_PLANT | STRONG_WATER,
  /* 005 */ WEAK_WATER | WEAK_STONE | STRONG_PLANT | STRONG_FIRE,
  /* 008 */ WEAK_PLANT | STRONG_FIRE | STRONG_WATER,
  /* 003 */ WEAK_FIRE | STRONG_PLANT | STRONG_WATER,
  /* 006 */ WEAK_WATER | WEAK_STONE | STRONG_PLANT | STRONG_FIRE,
  /* 009 */ WEAK_PLANT | STRONG_FIRE | STRONG_WATER,
  /* 025 */ 0,
  /* 026 */ 0,
  /* 063 */ 0,
  /* 064 */ 0,
  /* 065 */ 0,
  /* 086 */ WEAK_PLANT | STRONG_WATER,
  /* 087 */ WEAK_PLANT | WEAK_FIRE | STRONG_WATER,
  /* 092 */ STRONG_PLANT,
  /* 093 */ STRONG_PLANT,
  /* 094 */ STRONG_PLANT,
  /* 132 */ 0,
  /* 133 */ 0,
  /* 134 */ WEAK_PLANT | STRONG_FIRE | STRONG_WATER,
  /* 136 */ WEAK_WATER | WEAK_STONE | STRONG_PLANT | STRONG_FIRE,
  /* 196 */ 0,
  /* 135 */ 0,
  /* 151 */ 0,
  /* 197 */ 0,
  /* 203 */ 0,
  /* 213 */ WEAK_WATER | WEAK_STONE,
  /* 214 */ WEAK_FIRE | STRONG_PLANT,
  /* 225 */ WEAK_FIRE | WEAK_STONE | STRONG_PLANT,
  /* 235 */ 0,
  /* 241 */ 0,
  /* 251 */ WEAK_FIRE | STRONG_PLANT | STRONG_WATER,
  /* 172 */ 0,
  /* 175 */ 0,
  /* 176 */ WEAK_STONE | STRONG_PLANT,
  /* 144 */ WEAK_FIRE | WEAK_STONE | STRONG_PLANT,
  /* 145 */ WEAK_STONE | STRONG_PLANT,
  /* 146 */ WEAK_WATER | WEAK_STONE | STRONG_PLANT | STRONG_FIRE,
  /* 249 */ WEAK_STONE | STRONG_PLANT,
  /* 250 */ WEAK_WATER | WEAK_STONE | STRONG_PLANT | STRONG_FIRE,
  /* 150 */ 0,
  /* 039 */ 0,
  /* 040 */ 0,
  /* 174 */ 0,
  /* 143 */ 0,
  /* 216 */ 0,
  /* 217 */ 0,
  /* 243 */ 0,
  /* 244 */ WEAK_WATER | WEAK_STONE | STRONG_PLANT | STRONG_FIRE,
  /* 245 */ WEAK_PLANT | STRONG_FIRE | STRONG_WATER,
  /* 185 */ WEAK_PLANT | WEAK_WATER | STRONG_FIRE,
  /* 125 */ 0,
  /* 239 */ 0,
  /* 200 */ 0,
  /* 201 */ 0,
};

static int level(State* state) {
  const int allyLevel = state->ally->level_final();
  int diff = allyLevel - state->enemy->level_final();
  if (allyLevel <= 100) {
    diff += 100;
  }
  if (state->ally->type <= RESOURCE_ID_a142) {
    diff += ((state->ally->type - 1) / 3) * 10;
  }
  if (diff < 50) {
    return 50;
  }
  if (diff > 200) {
    return 200;
  }
  return diff;
}

static int effective(State* state) {
  const uint16_t type = types[state->enemy->type - ENEMY_OFFSET];
  switch (state->ally->type) {
    case RESOURCE_ID_a1:
    case RESOURCE_ID_a2:
    case RESOURCE_ID_a3:
      if (type & WEAK_PLANT) {
        return 5;
      }
      if (type & STRONG_PLANT) {
        return 2;
      }
      break;
    case RESOURCE_ID_a4:
    case RESOURCE_ID_a5:
    case RESOURCE_ID_a6:
      if (type & WEAK_FIRE) {
        return 5;
      }
      if (type & STRONG_FIRE) {
        return 2;
      }
      break;
    case RESOURCE_ID_a7:
    case RESOURCE_ID_a8:
    case RESOURCE_ID_a9:
      if (type & WEAK_WATER) {
        return 5;
      }
      if (type & STRONG_WATER) {
        return 2;
      }
      break;
    case RESOURCE_ID_a142:
      if (type & WEAK_STONE) {
        return 5;
      }
      break;
    default:
      return rand() % 6 + 1;
  }
  return 3;
}

bool game_damage(State* state) {
  if (state->health->restful_sleep_hour || state->enemy->type == RESOURCE_ID_egg) {
    return false;
  }
  state->enemy->health -= (7 * state->health->active_hour + 5400) * level(state) * effective(state) / 108000;
  return true;
}
