#include <pebble.h>
#include "event.h"
#include "const.h"
#define VERSION 1
#define VERSION_KEY 400
#define VALUE_KEY 401
#define ID_KEY 402

static EventValue event = EVENT_NONE;
static int id = -1;

EventValue* event_init() {
  if (persist_exists(VERSION_KEY) && persist_read_int(VERSION_KEY) == VERSION) {
    event = persist_read_int(VALUE_KEY);
    id = persist_read_int(ID_KEY);
  }
  return &event;
}

void event_next(Enemy *enemy, Health health, int identifier) {
  if (identifier == id) {
    return;
  }
  id = identifier;
  int check = rand() % 5;
  if (health.restful_sleep_hour) {
    event = EVENT_SLEEP;
    if (enemy->morph && enemy->type != RESOURCE_ID_132 && check == 4) {
      event |= EVENT_MORPH;
    }
    return;
  }
  event = EVENT_NONE;
  if (enemy->type == RESOURCE_ID_egg) {
    return;
  }
  switch (check) {
    case 0:
      event |= EVENT_EGG;
      break;
    case 1:
      if (enemy->level_multiplier > 1) {
        event |= EVENT_GHOST;
      }
      break;
  }
  if (enemy->morph && check == 4) {
    event |= EVENT_MORPH;
    return;
  }
  switch (enemy->type) {
    case RESOURCE_ID_1:
    case RESOURCE_ID_2:
    case RESOURCE_ID_4:
    case RESOURCE_ID_5:
    case RESOURCE_ID_7:
    case RESOURCE_ID_8:
    case RESOURCE_ID_86:
    case RESOURCE_ID_133:
      if (check < enemy->hours_alive) {
        event |= EVENT_EVO;
      }
      break;
  }
}

void event_deinit() {
  persist_write_int(VERSION_KEY, VERSION);
  persist_write_int(VALUE_KEY, event);
  persist_write_int(ID_KEY, id);
}
