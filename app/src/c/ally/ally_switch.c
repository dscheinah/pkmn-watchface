#include <pebble.h>
#include "ally.h"

void ally_switch(Ally* ally, ResourceValue target) {
  if (target == 4) {
    ally->selected = 0;
    ally->type = RESOURCE_ID_a201x;
    return;
  }
  ally->selected = target;
  if (target) {
    ally->type = target;
  }
}
