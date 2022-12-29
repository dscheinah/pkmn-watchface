#pragma once
#include <pebble.h>
#include "../state/const.h"
#include "../enemy/enemy.h"
#include "../health/health.h"

EventValue* event_init();

void event_next(Enemy *enemy, Health health, int identifier);

void event_deinit();
