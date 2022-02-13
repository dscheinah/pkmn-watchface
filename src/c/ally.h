#pragma once

typedef struct ally {
 int type;
 int shiny;
 int level;
 int level_modifier;
 int (*level_final)();
 int health;
 float experience;
} Ally;

Ally* ally_init();

void ally_reset();

int ally_evolution();

void ally_deinit();
