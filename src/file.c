#include <malloc.h>
#include <string.h>
#include "../file.h"


int no_special(const struct dirent *entry)
{
    int result = strcmp(entry->d_name, ".") == 0 ||
                 strcmp(entry->d_name, "..") == 0;

    return !result;
}


void change_dir(char *dir_name,
                const char *new_name,
                struct entries *entries)
{
    size_t len = strlen(new_name);

    if (len > MAX_DIR_LEN)
    {
        len = MAX_DIR_LEN;
    }

    strncpy(dir_name,
            new_name,
            len);

    if (entries->list)
    {
        free(entries->list);
    }

    entries->len = scandir(dir_name,
                           &entries->list,
                           no_special,
                           alphasort);

    entries->cursor = 0;
}
