#include <string.h>
#include <unistd.h>
#include "../input.h"


#define KEY_Q_LOW   'q'
#define KEY_UP      "\033[A"
#define KEY_DOWN    "\033[B"

void get_input(char *key)
{
    memset(key, 0, KEY_LEN);

    read(STDIN_FILENO,
         key,
         KEY_LEN);
}


bool should_exit(const char *key)
{
    return key[0] == KEY_Q_LOW;
}

void move_cursor(const char *key,
                 struct entries *entries)
{
    if (strcmp(key,
               KEY_UP) == 0)
    {
        entries->cursor--;

        if (entries->cursor < 0)
        {
            entries->cursor = entries->len - 1;
        }
    }
    else if (strcmp(key,
                    KEY_DOWN) == 0)
    {
        entries->cursor++;

        if (entries->cursor >= entries->len)
        {
            entries->cursor = 0;
        }
    }
}
