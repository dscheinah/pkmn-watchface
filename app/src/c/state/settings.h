#pragma once
#include "global.h"

void settings_init(State* state);

void settings_set(State* state, DictionaryIterator* iter);

void settings_quiet_changed(State* state);
