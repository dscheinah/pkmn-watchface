#include "game.h"
#include "../enemy/enemy.h"
#include "../state/settings.h"

void game_init(State* state) {
  game_level_set_ally(state);
  game_level_set_enemy(state);
  settings_quiet_changed();
  switch (state->quiet) {
    case QUIET_ON:
      if (enemy_quiet_enable(state)) {
        state->ally->level_modifier++;
      }
      break;
    case QUIET_OFF:
      if (enemy_quiet_disable(state)) {
        state->ally->level_modifier++;
      }
      break;
  }
}

bool game_tick(State* state) {
  if (game_damage(state) && (enemy_reset_bird(state) || enemy_reset(state))) {
    state->ally->level_modifier += 2;
    return false;
  }
  if (enemy_evolution(state)) {
    state->ally->level_modifier++;
    return false;
  }
  if (enemy_hatch_bird(state)) {
    state->ally->level_modifier++;
    return false;
  }
  return true;
}

void game_reset(State* state) {
  if (enemy_evolution_night(state) || enemy_hatch(state)) {
    state->ally->level_modifier++;
  }
}
