#include <string.h>
#include <unistd.h>
#include "../input.h"


#define KEY_Q_LOW   'q'
#define KEY_UP      "\033[A"
#define KEY_DOWN    "\033[B"
#define KEY_RIGHT   "\033[C"
#define KEY_LEFT    "\033[D"

void get_input(char *key)
{
    memset(key, 0, KEY_LEN);

    static int i = 0;
    const char *table[] =
    {
        [0] = KEY_LEFT,
        [1] = KEY_LEFT,
        [2] = KEY_LEFT,
        [3] = KEY_LEFT,
        [4] = KEY_LEFT,
        [5] = KEY_RIGHT
    };

    const char *go = table[i++];

    strcpy(key,
           go);

//    read(STDIN_FILENO,
//         key,
//         KEY_LEN);
}


bool should_exit(const char *key)
{
    return key[0] == KEY_Q_LOW;
}


void move_cursor(const char *key,
                 struct directory *dir,
                 struct preview *pre,
                 int height)
{
    const int screen = height - 3;
    const int old = dir->cursor;

    if (dir->nmemb > 0)
    {
        if (strcmp(key,
                   KEY_UP) == 0)
        {
            dir->cursor--;

            if (dir->cursor < 0)
            {
                dir->cursor = dir->nmemb - 1;
            }
        }
        else if (strcmp(key,
                        KEY_DOWN) == 0)
        {
            dir->cursor++;

            if (dir->cursor >= dir->nmemb)
            {
                dir->cursor = 0;
            }
        }
    }

    int diff = screen - dir->cursor + dir->offset;

    if (dir->cursor == 0)
    {
        dir->offset = 0;
    }
    else if (diff < 0)
    {
        diff *= -1;

        dir->offset += diff;
    }
    else if (diff > screen)
    {
        dir->offset -= diff - screen;
    }

    if (old != dir->cursor)
    {
        load_preview(dir,
                     pre);
    }
}

void move_dir(const char *key,
              struct directory *dir,
              struct preview *pre)
{
    if (dir->nmemb > 0)
    {
        struct dirent *curr = dir->list[dir->cursor];

        if (strcmp(key,
                   KEY_RIGHT) == 0 && is_dir(curr))
        {
            step_in(curr->d_name,
                    dir,
                    pre);
        }
        else if (strcmp(key,
                        KEY_LEFT) == 0)
        {
            step_out(dir,
                     pre);
        }
    }
}
