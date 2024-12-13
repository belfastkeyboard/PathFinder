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
                 int left,
                 int right,
                 int height);

void move_dir(const key key,
              struct directory *dir,
              struct preview *pre,
              settings settings,
              int left,
              int right,
              int height);

void scroll_file(const key key,
                 struct preview *pre,
                 settings settings,
                 int left,
                 int right,
                 int height);