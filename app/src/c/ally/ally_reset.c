#include <pebble.h>
#include "ally.h"

void ally_reset(Ally* ally, int shiny_rate) {
  if (ally->selected) {
    ally->type = ally->selected;
  } else if (ally->type == RESOURCE_ID_a201x) {
    ally->type = rand() % 3 + 1;
  } else {
    ally->type = (ally->type + rand() % 2) % 3 + 1;
  }
  ally->shiny = rand() % shiny_rate == 0;
  ally->level_modifier = 0;
}
