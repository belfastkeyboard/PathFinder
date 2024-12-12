#pragma once

#include "input.h"

#define SETTINGS_HIDE (1 << 0)
#define SETTINGS_PREV (1 << 1)


int change_settings(settings *settings,
                    const key key);
