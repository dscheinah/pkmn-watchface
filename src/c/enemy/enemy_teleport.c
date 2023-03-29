#include <pebble.h>
#include "enemy.h"
#include "helper.h"

static void createTeleport(State* state, int resource, int level) {
  helper_evolve(state, resource, level, true);
  state->enemy->teleport = 100;
  state->enemy->teleport_type = resource;
}

static void returnTeleport(State* state) {
  bool isDefault = state->enemy->teleport_type == RESOURCE_ID_63;
  if (state->event) {
    if (isDefault) {
      helper_evolve(state, RESOURCE_ID_64, 2, true);
      state->enemy->health = state->enemy->teleport;
      state->enemy->teleport = 0;
    }
    return;
  }
  helper_evolve(state, state->enemy->teleport_type, isDefault ? 1 : 2, true);
  state->enemy->health = state->enemy->teleport;
}

void enemy_teleport(State* state) {
  if (state->enemy->health > 0) {
    if (state->enemy->type == state->enemy->teleport_type && state->enemy->teleport - 20 > state->enemy->health) {
      state->enemy->teleport = state->enemy->health;
      state->enemy->health = 0;
    }
    return;
  }
  if (state->enemy->type == state->enemy->teleport_type) {
    state->enemy->teleport = 0;
    return;
  }
  if (state->event & (EVENT_BOSS | EVENT_EGG | EVENT_GHOST)) {
    return;
  }
  if (state->enemy->teleport > 0) {
    returnTeleport(state);
    return;
  }
  if (state->event) {
    return;
  }
  switch (state->enemy->type) {
    case RESOURCE_ID_86:
    case RESOURCE_ID_133:
    case RESOURCE_ID_216:
      createTeleport(state, RESOURCE_ID_63, 1);
      break;
    case RESOURCE_ID_25:
    case RESOURCE_ID_26:
    case RESOURCE_ID_125:
    case RESOURCE_ID_135:
    case RESOURCE_ID_145:
    case RESOURCE_ID_172:
    case RESOURCE_ID_239:
      if (rand() % 12 == 0) {
        createTeleport(state, RESOURCE_ID_243, 2);
      }
      break;
    case RESOURCE_ID_4:
    case RESOURCE_ID_5:
    case RESOURCE_ID_6:
    case RESOURCE_ID_136:
    case RESOURCE_ID_146:
      if (rand() % 12 == 0) {
        createTeleport(state, RESOURCE_ID_244, 2);
      }
      break;
    case RESOURCE_ID_7:
    case RESOURCE_ID_8:
    case RESOURCE_ID_9:
    case RESOURCE_ID_134:
      if (rand() % 12 == 0) {
        createTeleport(state, RESOURCE_ID_245, 2);
      }
      break;
  }
}
