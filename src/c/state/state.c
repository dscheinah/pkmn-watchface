#include "state.h"

#define VERSION     0
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
  .settings = SETTINGS_DOW | SETTINGS_BLUETOOTH,
  .quiet = QUIET_NONE,
  .missing = false,
  .identifier = -1,
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
  if (persist_read_int(VERSION_KEY) == VERSION) {
    persist_read_data(STATE_KEY, &state, sizeof(State));
    persist_read_data(ALLY_KEY, &ally, sizeof(Ally));
    persist_read_data(ENEMY_KEY, &enemy, sizeof(Enemy));
    persist_read_data(HEALTH_KEY, &health, sizeof(Health));
  } else {
    state.index[0] = 1 << (RESOURCE_ID_133 - ENEMY_OFFSET);
    state.index[1] = 0;
  }
  ally.level_final = &ally_level_final;
  enemy.level_final = &enemy_level_final;
  state.ally = &ally;
  state.enemy = &enemy;
  state.health = &health;
  return &state;
}

void state_deinit() {
  persist_write_int(VERSION_KEY, VERSION);
  persist_write_data(STATE_KEY, &state, sizeof(State));
  persist_write_data(ALLY_KEY, &ally, sizeof(Ally));
  persist_write_data(ENEMY_KEY, &enemy, sizeof(Enemy));
  persist_write_data(HEALTH_KEY, &health, sizeof(Health));
}