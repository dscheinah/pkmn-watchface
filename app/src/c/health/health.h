#pragma once
#include <pebble.h>
#include "../state/global.h"

void health_refresh(Health* health, int identifier, bool reset);

void health_update(Health* health, bool reset);
