#pragma once
#include <pebble.h>
#include "health.h"
#define ENEMY_COUNT 31

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

bool enemy_reset(bool egg, bool ghost);

bool enemy_evolution(Health health, int event);

bool enemy_night();

bool enemy_charge();

bool enemy_hatch(Health health);

void enemy_deinit();
