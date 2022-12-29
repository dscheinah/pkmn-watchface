#pragma once
#include <pebble.h>

typedef struct health {
 HealthValue steps;
 HealthValue steps_yesterday;
 HealthValue steps_last;
 HealthValue steps_hour;
 HealthValue sleep;
 HealthValue restful_sleep;
 HealthValue restful_sleep_last;
 HealthValue restful_sleep_hour;
 HealthValue active;
 HealthValue active_last;
 HealthValue active_hour;
} Health;

void health_init();

void health_refresh(bool update_yesterday);

void health_set(HealthValue steps, HealthValue sleep, HealthValue restful_sleep, HealthValue active);

Health health_get_collected(bool update_relative, bool update_yesterday);

void health_deinit();
