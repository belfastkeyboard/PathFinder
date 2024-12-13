#pragma once

#include <dirent.h>


typedef int (*SortFunc)(const struct dirent **,
                        const struct dirent **);


int sort_alphabetical(const struct dirent **entry1,
                      const struct dirent **entry2);

int sort_type(const struct dirent **entry1,
              const struct dirent **entry2);

