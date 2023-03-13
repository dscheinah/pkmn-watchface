#include <pebble.h>
#include "health.h"
#define DEMO 1
#define DEMO_SLEEP 2

static int mode = 0;

void health_init() {
  const time_t start = time_start_of_today();
  const time_t end = time(NULL);
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

#if !defined(TEST)
void health_refresh(Health* health, bool reset) {
  if (mode & DEMO) {
    if (reset) {
      health->steps = rand() % 30000;
    }
  } else {
    health->steps = health_service_sum_today(HealthMetricStepCount);
    health->active = health_service_sum_today(HealthMetricActiveSeconds);
  }
  if (health->steps > health->steps_last) {
    health->steps_last = health->steps;
  }
  if (mode & DEMO_SLEEP) {
    if (reset) {
      health->sleep = rand() % 50000;
    }
  } else {
    health->sleep = health_service_sum_today(HealthMetricSleepSeconds);
    health->restful_sleep = health_service_sum_today(HealthMetricSleepRestfulSeconds);
  }
}
#endif

void health_update(Health* health, bool reset) {
  if (reset) {
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
