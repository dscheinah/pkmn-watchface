#include <pebble.h>
#include "health.h"
#define DEMO 1
#define DEMO_SLEEP 2

static int mode = 0;

void health_init() {
  time_t start = time_start_of_today();
  time_t end = time(NULL);
  int available = HealthServiceAccessibilityMaskAvailable
    & health_service_metric_accessible(HealthMetricStepCount, start, end)
    & health_service_metric_accessible(HealthMetricActiveSeconds, start, end);
  if (!available) {
    mode |= DEMO;
  }
  int sleepAvailable = HealthServiceAccessibilityMaskAvailable
    & health_service_metric_accessible(HealthMetricSleepSeconds, start, end)
    & health_service_metric_accessible(HealthMetricSleepRestfulSeconds, start, end);
  if (!sleepAvailable) {
    mode |= DEMO_SLEEP;
  }
}

void health_refresh(Health* health, bool update_yesterday) {
  HealthValue steps, sleep, restful, active;
  if (mode & DEMO) {
    steps = rand() % 1500;
    active = rand() % 900;
    if (!update_yesterday) {
      steps += health->steps;
      active += health->active;
    }
  } else {
    steps = health_service_sum_today(HealthMetricStepCount);
    active = health_service_sum_today(HealthMetricActiveSeconds);
  }
  if (mode & DEMO_SLEEP) {
    if (update_yesterday) {
      sleep = rand() % 30000;
      restful = rand() % 20000;
    } else {
      sleep = health->sleep;
      restful = health->restful_sleep;
    }
  } else {
    sleep = health_service_sum_today(HealthMetricSleepSeconds);
    restful = health_service_sum_today(HealthMetricSleepRestfulSeconds);
  }
  health_set(health, steps, sleep, restful, active);
}

void health_set(Health* health, HealthValue steps, HealthValue sleep, HealthValue restful_sleep, HealthValue active) {
  health->steps = steps;
  health->sleep = sleep;
  health->restful_sleep = restful_sleep;
  health->active = active;
}

void health_update(Health* health, bool update_relative, bool update_yesterday) {
  if (update_relative) {
    if (update_yesterday) {
      health->steps_yesterday = health->steps > health->steps_last ? health->steps : health->steps_last;
    }
    health->restful_sleep_hour = health->restful_sleep - health->restful_sleep_last;
    if (health->restful_sleep_hour < 0) {
      health->restful_sleep_hour = health->restful_sleep;
    }
    health->active_hour = health->active - health->active_last;
    if (health->active_hour < 0) {
      health->active_hour = health->active;
    }
    health->steps_last = health->steps;
    health->restful_sleep_last = health->restful_sleep;
    health->active_last = health->active;
  }
}
