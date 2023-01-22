#pragma once
#include <pebble.h>

#define ENEMY_COUNT 52
#define ENEMY_OFFSET 45

#define EVENT_NONE 0
#define EVENT_SLEEP 1
#define EVENT_EVO 2
#define EVENT_MORPH 4
#define EVENT_GHOST 8
#define EVENT_EGG 16
#define EVENT_BOSS 32

#define SETTINGS_BLUETOOTH 1
#define SETTINGS_TIME_FORMAT 2
#define SETTINGS_DATE_FORMAT 4
#define SETTINGS_SECONDS 8
#define SETTINGS_DOW 16
#define SETTINGS_TAPS 32
#define SETTINGS_CACHE 64
#define SETTINGS_VIBES 128
#define SETTINGS_DARK 256

#define QUIET_OFF -2
#define QUIET_TOGGLE_OFF -1
#define QUIET_NONE 0
#define QUIET_TOGGLE_ON 1
#define QUIET_ON 2

#define DARK_OFF 0
#define DARK_ON 1
#define DARK_FULL 2

typedef uint16_t ResourceValue;

typedef struct {
 ResourceValue type;
 uint16_t level;
 uint16_t level_modifier;
 short health;
 uint16_t experience;
 bool shiny;
 ResourceValue selected;
 int (*level_final)();
} Ally;

typedef struct {
  ResourceValue type;
  uint16_t level;
  uint16_t level_multiplier;
  short health;
  uint16_t hours_alive;
  uint16_t index_count;
  bool morph;
  short teleport;
  ResourceValue teleport_type;
  int (*level_final)();
} Enemy;

typedef struct {
 HealthValue steps;
 HealthValue steps_yesterday;
 HealthValue steps_last;
 HealthValue sleep;
 HealthValue restful_sleep;
 HealthValue restful_sleep_last;
 HealthValue restful_sleep_hour;
 HealthValue active;
 HealthValue active_last;
 HealthValue active_hour;
} Health;

typedef uint16_t EventValue;

typedef uint16_t SettingsValue;

typedef short QuietValue;

typedef struct {
  Ally* ally;
  Enemy* enemy;
  Health* health;
  EventValue event;
  SettingsValue settings;
  QuietValue quiet;
  uint index[2];
  bool missing;
  short identifier;
} State;

typedef int DarkValue;
