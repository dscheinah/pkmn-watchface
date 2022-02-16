#include <pebble.h>
#include "ally.h"
#include "enemy.h"
#include "health.h"

void game_set_ally_level(Ally *ally, Health health);

void game_set_enemy_level(Enemy *enemy, Health health);

bool game_deal_damage(Ally *ally, Enemy *enemy, Health health);
