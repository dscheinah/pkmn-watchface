#include "game.h"
#include "../ally/ally.h"
#include "../enemy/enemy.h"

static bool doTick(State* state) {
  if (game_damage(state)) {
    enemy_teleport(state);
    if (enemy_reset_bird(state) || enemy_reset(state)) {
      state->ally->level_modifier += 2;
      return false;
    }
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

void game_init(State* state) {
  game_level(state);
  switch (state->quiet) {
    case QUIET_TOGGLE_ON:
      if (enemy_quiet_enable(state)) {
        state->ally->level_modifier++;
      }
      break;
    case QUIET_TOGGLE_OFF:
      if (enemy_quiet_disable(state)) {
        state->ally->level_modifier++;
      }
      break;
  }
  ally_evolution(state->ally);
}

void game_tick(State* state, bool reset) {
  bool canResetEnemy = doTick(state);
  if (reset) {
    ally_reset(state->ally, ENEMY_COUNT - state->enemy->index_count + 10);
    if (canResetEnemy && (enemy_evolution_night(state) || enemy_hatch(state))) {
      state->ally->level_modifier++;
    }
  }
  ally_evolution(state->ally);
}
