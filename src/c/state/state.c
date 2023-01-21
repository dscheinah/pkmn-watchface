#include "state.h"

#define VERSION_KEY 0
#define STATE_KEY   1
#define ALLY_KEY    2
#define ENEMY_KEY   3
#define HEALTH_KEY  4

static Ally ally = {
  .type = RESOURCE_ID_a1,
  .shiny = false,
  .level = 1,
  .level_modifier = 0,
  .health = 100,
  .experience = 0,
  .selected = 0,
};
static Enemy enemy = {
  .type = RESOURCE_ID_133,
  .level = 1,
  .level_multiplier = 1,
  .health = 100,
  .hours_alive = 0,
  .morph = false,
  .index_count = 1,
};
static Health health = {
  .steps = 0,
  .steps_yesterday = 0,
  .steps_last = 0,
  .sleep = 0,
  .restful_sleep = 0,
  .restful_sleep_hour = 0,
  .restful_sleep_last = 0,
  .active = 0,
  .active_hour = 0,
  .active_last = 0,
};
static State state = {
  .event = EVENT_NONE,
  .settings = SETTINGS_DOW | SETTINGS_BLUETOOTH | SETTINGS_VIBES,
  .quiet = QUIET_NONE,
  .missing = false,
  .identifier = -1,
  .counter = 0,
};

static int ally_level_final() {
  int level = ally.level + ally.level_modifier;
  return level > 999 ? 999 : level;
}

static int enemy_level_final() {
  int level = enemy.level * enemy.level_multiplier;
  if (level < 1) {
    return 1;
  }
  return level > 999 ? 999 : level;
}

State* state_init() {
  if (persist_exists(VERSION_KEY)) {
    persist_read_data(STATE_KEY, &state, sizeof(State));
    persist_read_data(ALLY_KEY, &ally, sizeof(Ally));
    persist_read_data(ENEMY_KEY, &enemy, sizeof(Enemy));
    persist_read_data(HEALTH_KEY, &health, sizeof(Health));
    switch (persist_read_int(VERSION_KEY)) {
      case 0:
        ally.selected = 0;
      case 1:
        state.counter = 0;
    }
  } else {
    state.index[0] = 1 << (RESOURCE_ID_133 - ENEMY_OFFSET);
    state.index[1] = 0;
  }
  ally.level_final = &ally_level_final;
  enemy.level_final = &enemy_level_final;
  state.ally = &ally;
  state.enemy = &enemy;
  state.health = &health;
  state.counter++;
  return &state;
}

bool state_update_index() {
  if (!enemy.index_count) {
    state.index[0] = 0;
    state.index[1] = 0;
  }
  if (enemy.type < RESOURCE_ID_1) {
    return false;
  }
  int key = 0, chk = enemy.type - ENEMY_OFFSET;
  if (chk > 31) {
    key = 1;
    chk -= 32;
  }
  int pos = 1 << chk;
  if (!(state.index[key] & pos)) {
    state.index[key] = state.index[key] | pos;
    enemy.index_count++;
    return true;
  }
  return false;
}

void state_write() {
  persist_write_int(VERSION_KEY, 2);
  persist_write_data(STATE_KEY, &state, sizeof(State));
  persist_write_data(ALLY_KEY, &ally, sizeof(Ally));
  persist_write_data(ENEMY_KEY, &enemy, sizeof(Enemy));
  persist_write_data(HEALTH_KEY, &health, sizeof(Health));
}
