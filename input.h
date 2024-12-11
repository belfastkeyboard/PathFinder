#pragma once

#include <stdbool.h>
#include "file.h"

#define KEY_LEN 5


void get_input(char *key);


bool should_exit(const char *key);


void move_cursor(const char *key,
                 struct directory *dir,
                 struct preview *pre,
                 int height);

void move_dir(const char *key,
              struct directory *dir,
              struct preview *pre);
