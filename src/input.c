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

//    const char *table[] = {
//        KEY_LEFT,
//        KEY_LEFT,
//        KEY_DOWN,
//        KEY_DOWN,
//        KEY_DOWN,
//        KEY_DOWN,
//        KEY_DOWN,
//        KEY_DOWN
//    };
//
//    static size_t i = 0;
//
//    const char *select = table[i++];
//
//    strcpy(key,
//           select);

    read(STDIN_FILENO,
         key,
         KEY_LEN);
}


bool should_exit(const key key)
{
    return key[0] == KEY_Q_LOW;
}


void move_cursor(const key key,
                 struct directory *dir,
                 struct preview *pre,
                 const settings settings,
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
                     pre,
                     settings);
    }
}

void move_dir(const key key,
              struct directory *dir,
              struct preview *pre,
              const settings settings)
{
    if (dir->nmemb > 0)
    {
        struct dirent *curr = dir->list[dir->cursor];

        if (strcmp(key,
                   KEY_RIGHT) == 0 &&
                   is_dir(curr))
        {
            step_in(curr->d_name,
                    dir,
                    pre,
                    settings);
        }
        else if (strcmp(key,
                        KEY_LEFT) == 0)
        {
            step_out(dir,
                     pre,
                     settings);
        }
    }
}
