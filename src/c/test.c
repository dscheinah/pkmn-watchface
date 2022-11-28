#include <pebble.h>
#include "test.h"
#include "const.h"
#include "health.h"

static int tick = 0;
static HealthValue steps = 0, sleep = 0, restful_sleep = 0, active = 0;
static bool day = true;

static void doReset() {
  steps = rand() % 7000 + 5000;
  sleep = rand() % 40000 + 20000;
  restful_sleep = active = 0;
}

static void doKill() {
  active += 30000;
  restful_sleep = 0;
}

static void prepareHatch(EventValue *event, int target) {
  doKill();
  *event = EVENT_EGG;
  steps = target;
}

void test_next_tick(Enemy *enemy, EventValue *event) {
  day = false;
  *event = EVENT_NONE;
  switch (tick++ % 32) {
    case 0:
      steps = 0, sleep = 0, restful_sleep = 0, active = 0;
      doKill();
      day = true;
      break;
    case 1:
    case 2:
      steps += 6000;
      sleep += 25000;
      *event = EVENT_SLEEP;
      restful_sleep++;
      break;
    case 3:
      prepareHatch(event, 5000);
      break;
    case 4:
    case 7:
    case 11:
    case 13:
      doReset();
      day = true;
      break;
    case 5:
    case 16:
      day = true;
      break;
    case 6:
      prepareHatch(event, 12000);
      break;
    case 8:
    case 9:
    case 18:
    case 20:
      enemy->hours_alive = rand() % 10;
      *event = EVENT_EVO;
      break;
    case 10:
      prepareHatch(event, 20000);
      break;
    case 12:
      prepareHatch(event, 26000);
      break;
    case 14:
      doKill();
      *event = EVENT_GHOST;
      break;
    case 15:
    case 19:
    case 21:
    case 22:
      doKill();
      break;
    case 17:
      enemy->hours_alive = 20;
      doKill();
      break;
    case 23:
      enemy->morph = true;
      break;
    case 31:
      doKill();
      *event = EVENT_BOSS;
      break;
    default:
      if (enemy->morph) {
        *event = EVENT_MORPH;
      }
      active += rand() % 900;
      break;
  }
}

void test_health_refresh() {
  health_set(steps, sleep, restful_sleep, active);
}

bool test_day() {
  return day;
}
