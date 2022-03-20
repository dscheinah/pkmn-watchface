#pragma once
#include <pebble.h>
#include "const.h"
#include "enemy.h"
#include "health.h"

EventValue* event_init();

void event_next(Enemy *enemy, Health health, int identifier);

void event_deinit();
