#pragma once
#include <pebble.h>
#include "../state/global.h"

void ally_reset(Ally* ally, int shiny_rate);

void ally_evolution(Ally* ally);

void ally_switch(Ally* ally, ResourceValue target);
