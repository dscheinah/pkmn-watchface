#include "game.h"
#include "event.h"
#include "../ally/ally.h"
#include "../enemy/enemy.h"
#include "../state/settings.h"

void game_init(Ally *ally, Enemy *enemy, Health health) {
  game_level_set_ally(ally, health);
  game_level_set_enemy(enemy, health);
  switch (settings_quiet_changed()) {
    case SETTINGS_QUIET_ON:
      if (enemy_quiet_enable(enemy, health)) {
        ally->level_modifier++;
      }
      break;
    case SETTINGS_QUIET_OFF:
      if (enemy_quiet_disable(enemy)) {
        ally->level_modifier++;
      }
      break;
  }
}

bool game_tick(Ally *ally, Enemy *enemy, Health health, EventValue event) {
  if (game_damage(ally, enemy, health) && (enemy_reset_bird(enemy, ally) || enemy_reset(enemy, event))) {
    ally->level_modifier += 2;
    return false;
  }
  if (enemy_evolution(enemy, event)) {
    ally->level_modifier++;
    return false;
  }
  if (enemy_hatch_bird(enemy, ally)) {
    ally->level_modifier++;
    return false;
  }
  return true;
}

void game_reset(Ally *ally, Enemy *enemy, Health health) {
  if (enemy_evolution_night(enemy) || enemy_hatch(enemy, health)) {
    ally->level_modifier++;
  }
}
