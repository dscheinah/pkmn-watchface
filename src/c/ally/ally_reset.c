#include <pebble.h>
#include "ally.h"

void ally_reset(Ally* ally, int shiny_rate) {
  ally->type = (ally->type + rand() % 2) % 3 + 1;
  ally->shiny = rand() % shiny_rate == 0;
  ally->level_modifier = 0;
}
