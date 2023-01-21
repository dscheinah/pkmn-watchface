#include <pebble.h>
#include "enemy.h"
#include "helper.h"

static void createTeleport(State* state, int resource, int level) {
  helper_evolve(state, resource, level, true);
  state->enemy->teleport = 100;
  state->enemy->teleport_type = resource;
}

static void returnTeleport(State* state, int resource, int level) {
  helper_evolve(state, resource, level, true);
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
  if (state->event & (EVENT_GHOST | EVENT_EGG | EVENT_BOSS)) {
    return;
  }
  if (state->enemy->teleport > 0) {
    if (state->enemy->teleport_type == RESOURCE_ID_63) {
      if (state->event & EVENT_EVO) {
        returnTeleport(state, RESOURCE_ID_64, 2);
        state->enemy->teleport = 0;
        return;
      }
      returnTeleport(state, RESOURCE_ID_63, 1);
      return;
    }
    returnTeleport(state, state->enemy->teleport_type, 3);
    return;
  }
  if (state->event) {
    return;
  }
  switch (state->enemy->type) {
    case RESOURCE_ID_133:
      createTeleport(state, RESOURCE_ID_63, 1);
      break;
  }
}
