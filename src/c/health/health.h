#pragma once
#include <pebble.h>
#include "../state/global.h"

void health_init();

void health_refresh(Health* health, bool reset);

void health_update(Health* health, bool reset);
