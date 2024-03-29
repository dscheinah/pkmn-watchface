#if defined(TEST)
#include <pebble.h>
#include "test.h"
#include "game/event.h"
#include "health/health.h"
#include "state/settings.h"

typedef struct {
  int check;
  HealthValue steps;
  HealthValue active;
  HealthValue sleep;
  HealthValue restful_sleep;
  bool day;
  QuietValue quiet;
  int hours_alive;
  int index_count;
  bool morph;
  int counter;
} Test;

static Test ticks[] = {
  // reset to zero
  {
    .day = true,
  },
  // test ally evolutions
  {
    .active = 30000,
    .day = true,
  },
  {
    .steps = 7500,
    .sleep = 25000,
    .restful_sleep = 1,
  },
  {
    .steps = 7500,
    .sleep = 25000,
  },
  // test ally quiet
  {
    .active = 30000,
    .day = true,
    .quiet = QUIET_ON,
  },
  {
    .steps = 7500,
    .sleep = 25000,
    .quiet = QUIET_ON,
    .restful_sleep = 1,
  },
  {
    .steps = 7500,
    .sleep = 25000,
    .quiet = QUIET_ON,
  },
  // test counter
  {
    .active = 30000,
    .counter = 600,
  },
  // hatch to quiet and evolve
  {
    .check = 1,
    .steps = 7500,
    .active = 30000,
    .sleep = 25000,
    .day = true,
  },
  {
    .quiet = QUIET_TOGGLE_ON,
  },
  {
    .quiet = QUIET_TOGGLE_OFF,
  },
  // hatch to sleep and evolve
  {
    .check = 1,
    .steps = 7500,
    .active = 30000,
    .sleep = 25000,
    .day = true,
  },
  {
    .restful_sleep = 1,
  },
  {
    .restful_sleep = 1,
  },
  {
    .quiet = QUIET_TOGGLE_ON,
  },
  {
    .check = 1,
    .steps = 7500,
    .active = 30000,
    .sleep = 25000,
    .day = true,
  },
  {
    .restful_sleep = 1,
  },
  {
    .restful_sleep = 1,
  },
  {
    .quiet = QUIET_TOGGLE_OFF,
  },
  {
    .sleep = 25000,
    .day = true,
  },
  // hatch to first
  {
    .check = 1,
    .steps = 5000,
    .active = 30000,
  },
  {
    .sleep = 25000,
    .day = true,
  },
  // do night evolution
  {
    .sleep = 25000,
    .day = true,
  },
  // hatch to second with evolutions
  {
    .check = 1,
    .steps = 15000,
    .active = 30000,
  },
  {
    .sleep = 25000,
    .day = true,
  },
  {
    .hours_alive = 5,
  },
  {
    .hours_alive = 5,
  },
  // hatch to third
  {
    .check = 1,
    .steps = 20000,
    .active = 30000,
  },
  {
    .sleep = 25000,
    .day = true,
  },
  // hatch to last
  {
    .check = 1,
    .steps = 25000,
    .active = 30000,
  },
  {
    .sleep = 25000,
    .day = true,
  },
  // create and evolve ghost
  {
    .check = 2,
    .active = 30000,
  },
  {
    .active = 30000,
  },
  {
    .steps = 7500,
    .sleep = 25000,
    .day = true,
  },
  // reset and evolve
  {
    .active = 30000,
  },
  {
    .hours_alive = 4,
  },
  // create and evolve frozen
  {
    .hours_alive = 20,
    .active = 30000,
  },
  {
    .hours_alive = 5,
  },
  // test long sleep trigger
  {
    .sleep = 50000,
    .active = 30000,
  },
  // double reset and vanish
  {
    .steps = 7500,
    .sleep = 25000,
    .day = true,
    .active = 30000,
  },
  {
    .active = 30000,
  },
  {
    .check = 10,
    .active = 600,
  },
  // return and vanish
  {
    .active = 30000,
  },
  {
    .check = 10,
    .active = 600,
  },
  // return evolved
  {
    .hours_alive = 5,
    .active = 30000,
  },
  // evolve last
  {
    .quiet = QUIET_TOGGLE_OFF,
  },
  // test reset with event and evolve
  {
    .steps = 7500,
    .active = 30000,
    .sleep = 25000,
    .day = true,
  },
  {
    .active = 30000,
    .hours_alive = 5,
  },
  {
    .hours_alive = 5,
  },
  // do some game ticks with morph
  {
    .steps = 7500,
    .active = 30000,
    .sleep = 25000,
    .day = true,
  },
  {
    .check = 3,
    .morph = true,
    .restful_sleep = 1,
  },
  {
    .check = 3,
    .active = 600,
    .morph = true,
  },
  {
    .check = 3,
    .active = 600,
    .morph = true,
  },
  {
    .check = 3,
    .active = 600,
    .morph = true,
  },
  {
    .check = 3,
    .active = 600,
    .morph = true,
  },
  // test hidden evolution
  {
    .steps = 30000,
  },
  // test boss
  {
    .index_count = ENEMY_COUNT,
    .active = 30000,
  },
};

static int tick = 0;
static int size = ARRAY_LENGTH(ticks);

int event_check() {
  return ticks[tick % size].check;
}

bool event_boss() {
  return true;
}

void health_refresh(Health* health, int identifier, bool reset) {
  if (reset) {
    health->steps = 0;
    health->active = 0;
    health->active_last = 0;
    health->active_hour = 0;
    health->sleep = 0;
    health->restful_sleep = 0;
    health->restful_sleep_last = 0;
    health->restful_sleep_hour = 0;
  }
  Test current = ticks[tick % size];
  health->steps += current.steps;
  health->active += current.active;
  health->sleep += current.sleep;
  health->restful_sleep += current.restful_sleep;
}

void settings_quiet_changed(State* state) {
  state->quiet = ticks[tick % size].quiet;
}

void test_next_tick(State* state) {
  Test current = ticks[++tick % size];
  state->enemy->hours_alive = current.hours_alive;
  state->enemy->morph = current.morph;
  state->counter = current.counter;
  if (current.index_count) {
    state->enemy->index_count = current.index_count;
  }
  state->identifier = -1;
}

bool test_day() {
  return ticks[tick % size].day;
}
#endif
