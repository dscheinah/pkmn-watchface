#pragma once
#include <pebble.h>

#define ENEMY_COUNT 40
#define ENEMY_OFFSET 25

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

#define QUIET_OFF -1
#define QUIET_NONE 0
#define QUIET_ON 1

typedef struct {
 int type;
 bool shiny;
 int level;
 int level_modifier;
 int (*level_final)();
 int health;
 int experience;
} Ally;

typedef struct {
  int type;
  int level;
  int level_multiplier;
  int (*level_final)();
  int health;
  int hours_alive;
  bool morph;
  int index_count;
} Enemy;

typedef struct {
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

typedef uint EventValue;

typedef uint SettingsValue;

typedef int QuietValue;

typedef struct {
  Ally* ally;
  Enemy* enemy;
  Health* health;
  EventValue event;
  SettingsValue settings;
  QuietValue quiet;
  uint index[2];
  bool missing;
  int identifier;
} State;
