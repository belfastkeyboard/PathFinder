#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../input.h"


#define KEY_Q_LOW   'q'
#define KEY_UP      "\033[A"
#define KEY_DOWN    "\033[B"
#define KEY_RIGHT   "\033[C"
#define KEY_LEFT    "\033[D"
#define KEY_PG_UP   "\033[5~"
#define KEY_PG_DN   "\033[6~"

void get_input(char *key)
{
    memset(key, 0, KEY_LEN);

//    static size_t i = 0;
//
//    if (i < 2)
//        strcpy(key,
//               KEY_LEFT);
//    else if (i < 9)
//        strcpy(key,
//               KEY_DOWN);
//    else
//        strcpy(key,
//               "o");
//
//    i++;

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
                 const int left,
                 const int right,
                 const int height)
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
                     settings,
                     left,
                     right,
                     height);
    }
}

void move_dir(const key key,
              struct directory *dir,
              struct preview *pre,
              const settings settings,
              const int left,
              const int right,
              const int height)
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
                    settings,
                    left,
                    right,
                    height);
        }
        else if (strcmp(key,
                        KEY_LEFT) == 0)
        {
            step_out(dir,
                     pre,
                     settings,
                     left,
                     right,
                     height);
        }
    }
}

void scroll_file(const key key,
                 struct preview *pre,
                 const settings settings,
                 const int left,
                 const int right,
                 const int height)
{
    if (pre->type & PT_FIL &&
        settings & SETTINGS_PREV)
    {
        const int width = right - left;

        if (strcmp(key,
                   KEY_PG_DN) == 0 &&
            pre->file.read < pre->file.size &&
            pre->file.offset < pre->file.size - width)
        {
            char *pos = memchr(pre->file.bytes,
                               '\n',
                               width);

            const long shift = (pos) ? pos - pre->file.bytes + 1 :
                                       width - 1;

            pre->file.offset += shift;

            load_file(pre,
                      pre->file.size,
                      left,
                      right,
                      height);
        }
        else if (strcmp(key,
                        KEY_PG_UP) == 0 &&
                 pre->file.offset > 0)
        {
            char buffer[width + 1];

            const long prev = (pre->file.offset - width > 0) ? pre->file.offset - width :
                                                               0;

            FILE *file = fopen(pre->path,
                               "r");

            fseek(file,
                  prev,
                  SEEK_SET);

            fread(buffer,
                  sizeof(char),
                  width,
                  file);

            buffer[width] = '\0';

            fclose(file);

            char *new_line = memchr(buffer,
                                    '\n',
                                    width);

            const long shift = (new_line) ? width - (buffer - new_line) :
                                            width - 1;

            pre->file.offset -= shift;

            load_file(pre,
                      pre->file.size,
                      left,
                      right,
                      height);
        }
    }
}
