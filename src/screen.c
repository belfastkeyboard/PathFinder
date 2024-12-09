#include <dirent.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "../screen.h"

void new_screen(void)
{
    printf("\033[?1049h");
    printf("\033[2J\033[H");
    fflush(stdout);
}

void close_screen(void)
{
    printf("\033[?1049l");
}


void enable_raw_mode(void)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &raw);

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(void)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &raw);

    raw.c_lflag |= (ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


void enable_bold(void)
{
    printf("\033[1m");
}

void reset_formatting(void)
{
    printf("\033[0m");
}


void print_directory(const struct entries *entries)
{
    for (int i = 0; i < entries->len; i++)
    {
        if(i == entries->cursor)
        {
            enable_bold();
        }

        printf("%s\n", entries->list[i]->d_name);

        if (i == entries->cursor)
        {
            reset_formatting();
        }
    }
}
