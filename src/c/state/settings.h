#pragma once
#include "global.h"

void settings_init(State* stateRef);

void settings_set(SettingsValue settings);

void settings_quiet_changed();
