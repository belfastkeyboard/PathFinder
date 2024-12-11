#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../path.h"
#include "../screen.h"
#include "../ext.h"

#define SIZE_LEN    8

#define ICON_FOLDER "📁"
#define ICON_FILE   "📄"
#define ICON_DENY   "🔒"


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


int screen_width(void)
{
    struct winsize window;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

    int width = window.ws_col;

    if (width == 0)
    {
        width = 80;
    }

    return width;
}

int screen_height(void)
{
    struct winsize window;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);

    int height = window.ws_row;

    if (height == 0)
    {
        height = 24;
    }

    return height;
}


void enable_raw_mode(void)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &raw);

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    printf("\033[?25l");
    fflush(stdout);
}

void disable_raw_mode(void)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &raw);

    raw.c_lflag |= (ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    printf("\033[?25h");
    fflush(stdout);
}


void print_vertical_line(const int x,
                         const int height)
{
    for (int i = 0; i < height; i++)
    {
        printf("\033[%d;%dH│",
               i + 1,
               x);
    }

    fflush(stdout);
}


void enable_bold(void)
{
    printf("\033[1m");
}

void reset_formatting(void)
{
    printf("\033[0m");
}


static void print_centred(const char *text,
                          const int left,
                          const int right)
{
    const size_t len = strlen(text);
    const int width = right - left;
    const int pad = (int)(width - len) / 2;

    printf("\033[%dG%s",
           left + pad,
           text);
}


static void print_title(const char *text,
                        const int left,
                        const int right)
{
    enable_bold();

    printf("\033[1H");

    print_centred(text,
                  left,
                  right);

    reset_formatting();
}


static void print_file_info(const char *dir,
                            const struct dirent *entry,
                            const int left,
                            const int right,
                            const int index,
                            bool is_preview)
{
    char full_path[PATH_MAX] = { 0 };
    const char *fn = entry->d_name;

    strcpy(full_path,
           dir);

    path(full_path,
         fn);

    const size_t fn_len = strlen(fn);

    const int width = right - left;
    const int max_len = (is_preview) ? width - 2:
                                       width - SIZE_LEN - 4;

    const size_t print_len = (fn_len < max_len) ? fn_len :
                                                  max_len;

    printf("%s ",
           ICON_FILE);

    fwrite(fn,
           sizeof(char),
           print_len,
           stdout);

    if (fn_len > max_len)
    {
        int ellipsis = (is_preview) ? right + 1:
                                      right - SIZE_LEN;

        printf("\033[%d;%dH...",
               index,
               ellipsis - 3);
    }

    if (!is_preview)
    {
        const struct filesize sz = file_size(full_path);

        printf("\033[%d;%dH%s",
               index,
               right - SIZE_LEN + (SIZE_LEN - sz.len),
               sz.suffix);
    }
}

static void print_no_permissions(const int left,
                                 const int index)
{
    printf("\033[%d;%dH%s",
           index,
           left,
           ICON_DENY);
}

static void print_directory_info(const struct dirent *entry)
{
    printf("%s %s",
           ICON_FOLDER,
           entry->d_name);
}

static void print_file_contents(const struct preview *pre,
                                const int left,
                                const int right,
                                const int height)
{
    char *ext = strrchr(pre->path,
                        '.');

    if (!ext)
    {
        ext = pre->path;
    }
    else
    {
        ext++;
    }

    if (valid_filetype(ext,
                       strlen(ext)))
    {
        const int width = right - left;
        void *bytes = pre->file.bytes;
        size_t remain = pre->file.count;

        for (int i = 3; i < height; i++)
        {
            if (remain > pre->file.count)
            {
                break;
            }

            printf("\033[%d;%dH",
                   i,
                   left);

            const size_t seek = (width < remain) ? width :
                                                   remain;

            void *new_line = memchr(bytes,
                                    '\n',
                                    seek);

            size_t write = width;

            if (new_line &&
                new_line - bytes < width)
            {
                write = new_line - bytes + 1;
            }

            write = fwrite(bytes,
                           sizeof(char),
                           write,
                           stdout);

            remain -= write;
            bytes += write;
        }
    }
    else
    {
        printf("Cannot display that file");
    }

    fflush(stdout);
}


void print_directory(const struct directory *dir,
                     const int left,
                     const int right,
                     const int height,
                     bool is_preview)
{
    if (!is_preview)
    {
        print_title(dir->title,
                    left,
                    right);
    }

    const int screen = height - 2;
    const int halt = (dir->nmemb < screen) ? dir->nmemb :
                                             dir->offset + screen;

    for (int i = dir->offset, j = 3; i < halt; i++, j++)
    {
        char full_path[PATH_MAX] = { 0 };

        struct dirent *entry = dir->list[i];

        strcpy(full_path,
               dir->path);

        printf("\033[%d;%dH",
               j,
               left);

        if (i == dir->cursor)
        {
            enable_bold();
        }

        if (is_dir(entry))
        {
            print_directory_info(entry);
        }
        else
        {
            print_file_info(dir->path,
                            entry,
                            left,
                            right,
                            j,
                            is_preview);
        }

        if (!has_permissions(path(full_path,
                                 entry->d_name),
                             R_OK))
        {
            print_no_permissions(left,
                                 j);
        }

        if (i == dir->cursor)
        {
            reset_formatting();
        }
    }

    fflush(stdout);
}

void print_preview(const struct preview *pre,
                   int left,
                   int right,
                   int height)
{
    char *title = strrchr(pre->path,
                          '/') + 1;

    print_title(title,
                left,
                right);

    if (pre->type & PT_DIR)
    {
        print_directory(&pre->directory,
                        left + 1,
                        right,
                        height,
                        true);
    }
    else if (pre->type & PT_FIL)
    {
        print_file_contents(pre,
                            left + 1,
                            right,
                            height);
    }
}
