#include <pebble.h>
#include "../state/global.h"

void multiplayer_init(State* state);

void multiplayer_handle_inbox(DictionaryIterator* iter);

void multiplayer_deinit();
