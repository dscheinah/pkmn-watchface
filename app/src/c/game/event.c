#include <pebble.h>
#include "event.h"

#if !defined(TEST)
int event_check() {
  return rand() % 5;
}

bool event_boss() {
  return rand() % 20 == 0;
}
#endif

void event_next(State* state, int identifier) {
  if (identifier == state->identifier) {
    return;
  }
  state->identifier = identifier;
  const int check = event_check();
  if (state->health->restful_sleep_hour) {
    state->event = EVENT_SLEEP;
    if (state->enemy->morph && state->enemy->type != RESOURCE_ID_132 && check == 3) {
      state->event |= EVENT_MORPH;
    }
    return;
  }
  state->event = EVENT_NONE;
  if (state->enemy->type == RESOURCE_ID_egg) {
    return;
  }
  if (state->enemy->type != RESOURCE_ID_150 && state->enemy->index_count >= ENEMY_COUNT - 1 && event_boss()) {
    state->event |= EVENT_BOSS;
  }
  switch (check) {
    case 1:
      state->event |= EVENT_EGG;
      break;
    case 2:
      if (state->enemy->level_multiplier > 1) {
        state->event |= EVENT_GHOST;
      }
      break;
    case 3:
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
    case RESOURCE_ID_216:
      if (check < state->enemy->hours_alive) {
        state->event |= EVENT_EVO;
      }
      break;
  }
}
