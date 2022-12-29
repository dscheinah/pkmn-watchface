#include <pebble.h>
#include "../ally/ally.h"
#include "../enemy/enemy.h"
#include "../health/health.h"

void game_level_set_ally(Ally *ally, Health health);

void game_level_set_enemy(Enemy *enemy, Health health);

bool game_damage(Ally *ally, Enemy *enemy, Health health);
