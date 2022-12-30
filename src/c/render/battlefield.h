#include <pebble.h>
#include "../state/const.h"
#include "../ally/ally.h"
#include "../enemy/enemy.h"

void battlefield_load(Layer *root, Ally *ally, Enemy *enemy, EventValue *event);

void battlefield_mark_dirty();

void battlefield_unload();
