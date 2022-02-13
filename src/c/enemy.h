#pragma once
#include "health.h"

typedef struct {
  int type;
  int level;
  int level_multiplier;
  int (*level_final)();
  int health;
  int hours_alive;
} Enemy;

Enemy* enemy_init();

int enemy_reset(int egg, int ghost);

int enemy_evolution(Health health);

int enemy_night();

int enemy_charge();

int enemy_hatch(Health health);

void enemy_deinit();
