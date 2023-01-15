#include <pebble.h>
#include "ally.h"

void ally_switch(Ally* ally, ResourceValue target) {
  ally->selected = target;
  if (!target || ally->type == RESOURCE_ID_a142) {
    return;
  }
  ally->type = target;
}
