#include <pebble.h>
#include "event.h"

void event_next(State* state, int identifier) {
  if (identifier == state->identifier) {
    return;
  }
  state->identifier = identifier;
  int check = rand() % 5;
  if (state->health->restful_sleep_hour) {
    state->event = EVENT_SLEEP;
    if (state->enemy->morph && state->enemy->type != RESOURCE_ID_132 && check == 4) {
      state->event |= EVENT_MORPH;
    }
    return;
  }
  state->event = EVENT_NONE;
  if (state->enemy->type == RESOURCE_ID_egg) {
    return;
  }
  if (state->enemy->type != RESOURCE_ID_150 && state->enemy->index_count >= ENEMY_COUNT - 1 && rand() % 20 == 0) {
    state->event |= EVENT_BOSS;
  }
  switch (check) {
    case 0:
      state->event |= EVENT_EGG;
      break;
    case 1:
      if (state->enemy->level_multiplier > 1) {
        state->event |= EVENT_GHOST;
      }
      break;
    case 4:
      if (state->enemy->morph) {
        state->event |= EVENT_MORPH;
        return;
      }
      break;
  }
  switch (state->enemy->type) {
    case RESOURCE_ID_1:
    case RESOURCE_ID_2:
    case RESOURCE_ID_4:
    case RESOURCE_ID_5:
    case RESOURCE_ID_7:
    case RESOURCE_ID_8:
    case RESOURCE_ID_86:
    case RESOURCE_ID_133:
      if (check < state->enemy->hours_alive) {
        state->event |= EVENT_EVO;
      }
      break;
  }
}
