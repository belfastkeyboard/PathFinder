#pragma once

#include <stdbool.h>
#include "file.h"
#include "types.h"
#include "settings.h"


#define KEY_LEN 5


void get_input(key key);


bool should_exit(const key key);


void move_cursor(const key key,
                 struct directory *dir,
                 struct preview *pre,
                 settings settings,
                 int height);

void move_dir(const key key,
              struct directory *dir,
              struct preview *pre,
              settings settings);
