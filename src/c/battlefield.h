#include <pebble.h>
#include "const.h"
#include "ally.h"
#include "enemy.h"

void battlefield_load(Layer *root, Ally *ally, Enemy *enemy, EventValue *event);

void battlefield_set_enemy_missing(bool missing);

void battlefield_mark_dirty();

void battlefield_unload();
