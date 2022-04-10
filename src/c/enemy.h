#pragma once
#include <pebble.h>
#include "const.h"
#include "health.h"

typedef struct {
  int type;
  int level;
  int level_multiplier;
  int (*level_final)();
  int health;
  int hours_alive;
  bool morph;
  int index_count;
  uint index[2];
} Enemy;

Enemy* enemy_init();

bool enemy_reset(EventValue event);

bool enemy_evolution(EventValue event);

bool enemy_night();

bool enemy_charge();

bool enemy_hatch(Health health);

bool enemy_quiet(bool quiet, Health health);

void enemy_deinit();
