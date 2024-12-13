#include <ctype.h>
#include "../sort.h"


int sort_alphabetical(const struct dirent **entry1,
                      const struct dirent **entry2)
{
    const char *a = (*entry1)->d_name;
    const char *b = (*entry2)->d_name;

    while (*a == *b)
    {
        a++;
        b++;
    }

    const int x = tolower(*a);
    const int y = tolower(*b);

    return ((x > y) - (x < y));
}

int sort_type(const struct dirent **entry1,
              const struct dirent **entry2)
{
    const unsigned char x = (*entry1)->d_type;
    const unsigned char y = (*entry2)->d_type;

    return ((x > y) - (x < y));
}
