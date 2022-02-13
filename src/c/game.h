#include "ally.h"
#include "enemy.h"
#include "health.h"

int game_set_ally_level(Ally *ally, Health health);

int game_set_enemy_level(Enemy *enemy, Health health);

int game_deal_damage(Ally *ally, Enemy *enemy, Health health);
