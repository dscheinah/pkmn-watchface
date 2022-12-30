#pragma once
#include <pebble.h>
#include "../state/global.h"

void health_init();

void health_refresh(Health* health, bool update_yesterday);

void health_set(Health* health, HealthValue steps, HealthValue sleep, HealthValue restful_sleep, HealthValue active);

void health_update(Health* health, bool update_relative, bool update_yesterday);
