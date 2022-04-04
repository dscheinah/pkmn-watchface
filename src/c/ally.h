#pragma once
#include <pebble.h>

typedef struct ally {
 int type;
 bool shiny;
 int level;
 int level_modifier;
 int (*level_final)();
 int health;
 float experience;
} Ally;

Ally* ally_init();

void ally_reset(int shiny_rate);

void ally_evolution();

void ally_deinit();
