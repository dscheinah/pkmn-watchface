#include <pebble.h>
#include "ally.h"
#define VERSION 2
#define VERSION_KEY 100
#define TYPE_KEY 101
#define LEVEL_KEY 102
#define SHINY_KEY 103

static Ally ally;

static int level_final() {
  int level = ally.level + ally.level_modifier;
  return level > 999 ? 999 : level;
}

Ally* ally_init() {
  ally.level = 1;
  ally.health = 100;
  ally.experience = 0;
  if (persist_exists(VERSION_KEY) && persist_read_int(VERSION_KEY) == VERSION) {
    ally.type = persist_read_int(TYPE_KEY);
    ally.shiny = persist_read_int(SHINY_KEY);
    ally.level_modifier = persist_read_int(LEVEL_KEY);
  } else {
    ally.type = rand() % 3 + 1;
    ally.shiny = 0;
    ally.level_modifier = 0;
  }
  ally.level_final = &level_final;
  return &ally;
}

void ally_reset(int shiny_rate) {
  int current = (ally.type - 1) % 3;
  int add = rand() % 2 + 1;
  ally.type = (current + add) % 3 + 1;
  ally.shiny = rand() % shiny_rate == 0;
  ally.level_modifier = 0;
}

void ally_evolution() {
  int level = level_final();
  if (ally.type < 4 && level > 15) {
    ally.type += 3;
  }
  if (ally.type < 7 && level > 35) {
    ally.type += 3;
  }
}

void ally_deinit() {
  persist_write_int(VERSION_KEY, VERSION);
  persist_write_int(TYPE_KEY, ally.type);
  persist_write_int(SHINY_KEY, ally.shiny);
  persist_write_int(LEVEL_KEY, ally.level_modifier);
}
