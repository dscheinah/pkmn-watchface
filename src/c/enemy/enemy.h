#pragma once
#include <pebble.h>
#include "../ally/ally.h"
#include "../state/const.h"
#include "../health/health.h"

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

bool enemy_reset(Enemy* enemy, EventValue event);

bool enemy_reset_bird(Enemy* enemy, Ally* ally);

bool enemy_evolution(Enemy* enemy, EventValue event);

bool enemy_evolution_night(Enemy* enemy);

bool enemy_hatch(Enemy* enemy, Health health);

bool enemy_hatch_bird(Enemy* enemy, Ally* ally);

bool enemy_charge(Enemy* enemy);

bool enemy_quiet_enable(Enemy* enemy, Health health);

bool enemy_quiet_disable(Enemy* enemy);

void enemy_deinit();
