#include <stddef.h>
#include <string.h>
#include "../file.h"
#include "../path.h"

static void cull_trailing_slash(char *path,
                                const size_t len)
{
    if (path[len - 1] == '/')
    {
        path[len - 1] = '\0';
    }
}


char *path(char *path,
           const char *segment)
{
    const size_t len = strlen(path);
    const size_t s_len = strlen(segment);
    int pad = 0;

    if (!is_root(path))
    {
        path[len] = '/';

        pad = 1;
    }

    strncpy(path + len + pad,
            segment,
            s_len + 1);

    cull_trailing_slash(path,
                        len + s_len + 1);

    return path;
}
