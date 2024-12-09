#pragma once

#include <stddef.h>
#include <dirent.h>

#define MAX_DIR_LEN 256



struct entries
{
    struct dirent **list;
    int len;
    int cursor;
};


int no_special(const struct dirent *entry);


void change_dir(char *dir_name,
                const char *new_name,
                struct entries *entries);
