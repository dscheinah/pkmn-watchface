#include <pebble.h>
#include "health.h"
#define VERSION 3
#define VERSION_KEY 300
#define DAY_KEY 301
#define LAST_STEPS_KEY 302
#define LAST_RESTFUL_KEY 303
#define LAST_ACTIVE_KEY 304
#define HOUR_RESTFUL_KEY 305
#define HOUR_ACTIVE_KEY 306

static Health health =  {
  .steps = 0,
  .steps_yesterday = 0,
  .steps_last = 0,
  .sleep = 0,
  .restful_sleep = 0,
  .restful_sleep_hour = 0,
  .restful_sleep_last = 0,
  .active = 0,
  .active_hour = 0,
  .active_last = 0,
};
static int demo = 0;

void health_init() {
  time_t start = time_start_of_today();
  time_t end = time(NULL);
  int available = HealthServiceAccessibilityMaskAvailable
    & health_service_metric_accessible(HealthMetricStepCount, start, end)
    & health_service_metric_accessible(HealthMetricSleepSeconds, start, end)
    & health_service_metric_accessible(HealthMetricSleepRestfulSeconds, start, end)
    & health_service_metric_accessible(HealthMetricActiveSeconds, start, end);
  if (!available) {
    demo = 1000;
  }
  if (persist_exists(VERSION_KEY) && persist_read_int(VERSION_KEY) == VERSION + demo) {
    health.steps_yesterday = persist_read_int(DAY_KEY);
    health.steps_last = persist_read_int(LAST_STEPS_KEY);
    health.restful_sleep_last = persist_read_int(LAST_RESTFUL_KEY);
    health.active_last = persist_read_int(LAST_ACTIVE_KEY);
    health.restful_sleep_hour = persist_read_int(HOUR_RESTFUL_KEY);
    health.active_hour = persist_read_int(HOUR_ACTIVE_KEY);
  }
}

void health_refresh(bool update_yesterday) {
  if (demo) {
    HealthValue steps = rand() % 1000, sleep, restful, active = rand() % 900;
    if (update_yesterday) {
      sleep = rand() % 30000;
      restful = rand() % 20000;
    } else {
      steps += health.steps;
      sleep = health.sleep;
      restful = health.restful_sleep;
      active += health.active;
    }
    health_set(steps, sleep, restful, active);
    return;
  }
  health_set(
    health_service_sum_today(HealthMetricStepCount),
    health_service_sum_today(HealthMetricSleepSeconds),
    health_service_sum_today(HealthMetricSleepRestfulSeconds),
    health_service_sum_today(HealthMetricActiveSeconds)
  );
}

void health_set(HealthValue steps, HealthValue sleep, HealthValue restful_sleep, HealthValue active) {
  health.steps = steps;
  health.sleep = sleep;
  health.restful_sleep = restful_sleep;
  health.active = active;
}

Health health_get_collected(bool update_relative, bool update_yesterday) {
  if (update_relative) {
    if (update_yesterday) {
      health.steps_yesterday = health.steps > health.steps_last ? health.steps : health.steps_last;
    }
    health.restful_sleep_hour = health.restful_sleep - health.restful_sleep_last;
    if (health.restful_sleep_hour < 0) {
      health.restful_sleep_hour = health.restful_sleep;
    }
    health.active_hour = health.active - health.active_last;
    if (health.active_hour < 0) {
      health.active_hour = health.active;
    }
    health.steps_last = health.steps;
    health.restful_sleep_last = health.restful_sleep;
    health.active_last = health.active;
  }
  return health;
}

void health_deinit() {
  persist_write_int(VERSION_KEY, VERSION + demo);
  persist_write_int(DAY_KEY, health.steps_yesterday);
  persist_write_int(LAST_STEPS_KEY, health.steps_last);
  persist_write_int(LAST_RESTFUL_KEY, health.restful_sleep_last);
  persist_write_int(LAST_ACTIVE_KEY, health.active_last);
  persist_write_int(HOUR_RESTFUL_KEY, health.restful_sleep_hour);
  persist_write_int(HOUR_ACTIVE_KEY, health.active_hour);
}
