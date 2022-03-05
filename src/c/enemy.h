#pragma once
#include <pebble.h>
#include "health.h"

typedef struct {
  int type;
  int level;
  int level_multiplier;
  int (*level_final)();
  int health;
  int hours_alive;
  bool morph;
} Enemy;

Enemy* enemy_init();

bool enemy_reset(bool egg, bool ghost);

bool enemy_evolution(Health health);

bool enemy_night();

bool enemy_charge();

bool enemy_hatch(Health health);

void enemy_deinit();
