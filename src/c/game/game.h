#pragma once
#include <pebble.h>
#include "../ally/ally.h"
#include "../enemy/enemy.h"
#include "../health/health.h"
#include "../state/const.h"

void game_level_set_ally(Ally *ally, Health health);

void game_level_set_enemy(Enemy *enemy, Health health);

bool game_damage(Ally *ally, Enemy *enemy, Health health);

void game_init(Ally *ally, Enemy *enemy, Health health);

bool game_tick(Ally *ally, Enemy *enemy, Health health, EventValue event);

void game_reset(Ally *ally, Enemy *enemy, Health health);
