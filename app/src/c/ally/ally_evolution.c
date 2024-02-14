#include <pebble.h>
#include "ally.h"

void ally_evolution(Ally* ally) {
  const int level = ally->level_final();
  if (level > 100) {
    ally->type = RESOURCE_ID_a142;
    return;
  }
  if (ally->type < 4 && level > 15) {
    ally->type += 3;
  }
  if ((ally->type < 7 && level > 35) || (ally->type == RESOURCE_ID_a2 && level > 31)) {
    ally->type += 3;
  }
}
