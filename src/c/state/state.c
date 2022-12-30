#include "state.h"
#include "../enemy/helper.h"

#define ALLY_VERSION 2
#define ALLY_VERSION_KEY 100
#define ALLY_TYPE_KEY 101
#define ALLY_LEVEL_KEY 102
#define ALLY_SHINY_KEY 103

#define ENEMY_VERSION 2
#define ENEMY_VERSION_KEY 200
#define ENEMY_TYPE_KEY 201
#define ENEMY_LEVEL_KEY 202
#define ENEMY_HEALTH_KEY 203
#define ENEMY_ALIVE_KEY 204
#define ENEMY_MORPH_KEY 205
#define ENEMY_INDEX_COUNT_KEY 206
#define ENEMY_INDEX_0_KEY 207
#define ENEMY_INDEX_1_KEY 208

#define HEALTH_VERSION 3
#define HEALTH_VERSION_KEY 300
#define HEALTH_DAY_KEY 301
#define HEALTH_LAST_STEPS_KEY 302
#define HEALTH_LAST_RESTFUL_KEY 303
#define HEALTH_LAST_ACTIVE_KEY 304
#define HEALTH_HOUR_RESTFUL_KEY 305
#define HEALTH_HOUR_ACTIVE_KEY 306

#define EVENT_VERSION 1
#define EVENT_VERSION_KEY 400
#define EVENT_VALUE_KEY 401
#define EVENT_ID_KEY 402

#define SETTINGS_VERSION 1
#define SETTINGS_VERSION_KEY 500
#define SETTINGS_FLAGS_KEY 501
#define SETTINGS_QUIET_KEY 502

static Ally ally = {
  .level = 1,
  .health = 100,
  .experience = 0,
};
static Enemy enemy = {
  .level = 1,
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
  .ally = &ally,
  .enemy = &enemy,
  .health = &health,
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
  if (persist_exists(ALLY_VERSION_KEY) && persist_read_int(ALLY_VERSION_KEY) == ALLY_VERSION) {
    ally.type = persist_read_int(ALLY_TYPE_KEY);
    ally.shiny = persist_read_bool(ALLY_SHINY_KEY);
    ally.level_modifier = persist_read_int(ALLY_LEVEL_KEY);
  } else {
    ally.type = rand() % 3 + 1;
    ally.shiny = 0;
    ally.level_modifier = 0;
  }
  ally.level_final = &ally_level_final;

  if (persist_exists(ENEMY_VERSION_KEY) && persist_read_int(ENEMY_VERSION_KEY) == ENEMY_VERSION) {
    enemy.type = persist_read_int(ENEMY_TYPE_KEY);
    enemy.level_multiplier = persist_read_int(ENEMY_LEVEL_KEY);
    enemy.health = persist_read_int(ENEMY_HEALTH_KEY);
    enemy.hours_alive = persist_read_int(ENEMY_ALIVE_KEY);
    enemy.morph = persist_read_bool(ENEMY_MORPH_KEY);
    enemy.index_count = persist_read_int(ENEMY_INDEX_COUNT_KEY);
    state.index[0] = persist_read_int(ENEMY_INDEX_0_KEY);
    state.index[1] = persist_read_int(ENEMY_INDEX_1_KEY);
  } else {
    enemy.index_count = 0;
    state.index[0] = 0;
    state.index[1] = 0;
    helper_evolve(&state, RESOURCE_ID_133, 1, true);
  }
  enemy.level_final = &enemy_level_final;

  if (persist_exists(HEALTH_VERSION_KEY) && persist_read_int(HEALTH_VERSION_KEY) == HEALTH_VERSION) {
    health.steps_yesterday = persist_read_int(HEALTH_DAY_KEY);
    health.steps_last = persist_read_int(HEALTH_LAST_STEPS_KEY);
    health.restful_sleep_last = persist_read_int(HEALTH_LAST_RESTFUL_KEY);
    health.active_last = persist_read_int(HEALTH_LAST_ACTIVE_KEY);
    health.restful_sleep_hour = persist_read_int(HEALTH_HOUR_RESTFUL_KEY);
    health.active_hour = persist_read_int(HEALTH_HOUR_ACTIVE_KEY);
  }

  if (persist_exists(EVENT_VERSION_KEY) && persist_read_int(EVENT_VERSION_KEY) == EVENT_VERSION) {
    state.event = persist_read_int(EVENT_VALUE_KEY);
    state.identifier = persist_read_int(EVENT_ID_KEY);
  }

  if (persist_exists(SETTINGS_VERSION_KEY) && persist_read_int(SETTINGS_VERSION_KEY) == SETTINGS_VERSION) {
    state.settings = persist_read_int(SETTINGS_FLAGS_KEY);
    state.quiet = persist_read_int(SETTINGS_QUIET_KEY);
  }

  return &state;
}

void state_deinit() {
  persist_write_int(ALLY_VERSION_KEY, ALLY_VERSION);
  persist_write_int(ALLY_TYPE_KEY, ally.type);
  persist_write_bool(ALLY_SHINY_KEY, ally.shiny);
  persist_write_int(ALLY_LEVEL_KEY, ally.level_modifier);

  persist_write_int(ENEMY_VERSION_KEY, ENEMY_VERSION);
  persist_write_int(ENEMY_TYPE_KEY, enemy.type);
  persist_write_int(ENEMY_LEVEL_KEY, enemy.level_multiplier);
  persist_write_int(ENEMY_HEALTH_KEY, enemy.health);
  persist_write_int(ENEMY_ALIVE_KEY, enemy.hours_alive);
  persist_write_bool(ENEMY_MORPH_KEY, enemy.morph);
  persist_write_int(ENEMY_INDEX_COUNT_KEY, enemy.index_count);
  persist_write_int(ENEMY_INDEX_0_KEY, state.index[0]);
  persist_write_int(ENEMY_INDEX_1_KEY, state.index[1]);

  persist_write_int(HEALTH_VERSION_KEY, HEALTH_VERSION);
  persist_write_int(HEALTH_DAY_KEY, health.steps_yesterday);
  persist_write_int(HEALTH_LAST_STEPS_KEY, health.steps_last);
  persist_write_int(HEALTH_LAST_RESTFUL_KEY, health.restful_sleep_last);
  persist_write_int(HEALTH_LAST_ACTIVE_KEY, health.active_last);
  persist_write_int(HEALTH_HOUR_RESTFUL_KEY, health.restful_sleep_hour);
  persist_write_int(HEALTH_HOUR_ACTIVE_KEY, health.active_hour);

  persist_write_int(EVENT_VERSION_KEY, EVENT_VERSION);
  persist_write_int(EVENT_VALUE_KEY, state.event);
  persist_write_int(EVENT_ID_KEY, state.identifier);

  persist_write_int(SETTINGS_VERSION_KEY, SETTINGS_VERSION);
  persist_write_int(SETTINGS_FLAGS_KEY, state.settings);
  persist_write_int(SETTINGS_QUIET_KEY, state.quiet);
}
