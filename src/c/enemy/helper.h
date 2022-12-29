#pragma once
#include <pebble.h>
#include "enemy.h"
#include "../ally/ally.h"

void helper_evolve(Enemy* enemy, int type, int level, bool reset);

int helper_bird(Enemy* enemy, Ally* ally);
