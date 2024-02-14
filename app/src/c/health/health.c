#include <pebble.h>
#include "health.h"

static bool available = PBL_IF_HEALTH_ELSE(true, false);

#if !defined(TEST)
void health_refresh(Health* health, int identifier, bool reset) {
  if (available) {
    health->steps = health_service_sum_today(HealthMetricStepCount);
    health->active = health_service_sum_today(HealthMetricActiveSeconds);
    health->sleep = health_service_sum_today(HealthMetricSleepSeconds);
    health->restful_sleep = health_service_sum_today(HealthMetricSleepRestfulSeconds);
  } else {
    if (reset) {
      health->sleep = rand() % 40000 + 10000;
      health->active = rand() % 550;
      health->restful_sleep = 0;
      health->steps = 0;
    }
    health->active_last = 0;
    if (identifier < 6 || rand() % 40 == 0) {
      health->restful_sleep++;
    } else {
      health->steps = (health->active / 10) * identifier * identifier;
    }
  }
  if (health->steps > health->steps_last) {
    health->steps_last = health->steps;
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
