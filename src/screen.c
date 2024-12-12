#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../path.h"
#include "../screen.h"
#include "../ext.h"

#define SIZE_LEN        8

#define ICON_FOLDER     "📁"
#define ICON_FILE       "📄"
#define ICON_DENY       "🔒"

#define COLOUR_INFO     51,  215, 204
#define COLOUR_ERROR    215, 29,  45


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


static void enable_bold(void)
{
    printf("\033[1m");
}

static void reset_formatting(void)
{
    printf("\033[0m");
}


static void set_text_colour(unsigned char r,
                            unsigned char g,
                            unsigned char b)
{
    printf("\033[38;2;%d;%d;%dm",
           r,
           g,
           b);
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


static void print_centred(const char *text,
                          const int left,
                          const int right)
{
    const size_t len = strlen(text);
    const int width = right - left;

    const size_t to_write = (len < width) ? len :
                                            width;

    size_t pad = (width - to_write) / 2;

    printf("\033[%zuG",
           left + pad);

    pad = fwrite(text,
                 sizeof(char),
                 to_write,
                 stdout);

    if (len > width)
    {
        printf("\033[%zuG...",
               left + pad - 3);
    }
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


static void print_entry_info(const char *name,
                             const char *symbol,
                             const char *path,
                             const int left,
                             const int right,
                             const int index,
                             bool is_file,
                             bool is_preview)
{
    const size_t fn_len = strlen(name);

    const int width = right - left;
    const int max_len = (is_preview) ? width - 2:
                                       width - SIZE_LEN - 4;

    const size_t print_len = (fn_len < max_len) ? fn_len :
                                                  max_len;

    printf("%s ",
           symbol);

    fwrite(name,
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

    if (is_file &&
        !is_preview)
    {
        const struct filesize sz = file_size(path);

        printf("\033[%d;%dH%s",
               index,
               right - SIZE_LEN + (SIZE_LEN - sz.len),
               sz.suffix);
    }
}


static void print_file_contents(const struct preview *pre,
                                const int left,
                                const int right,
                                const int height)
{
    const int width = right - left;
    void *bytes = pre->file.bytes;
    int remain = (int)(pre->file.count);

    for (int i = 3; i < height && remain; i++)
    {
        printf("\033[%d;%dH",
               i,
               left);

        const size_t seek = (width < remain) ? width :
                                               remain;

        void *new_line = memchr(bytes,
                                '\n',
                                seek);

        size_t write = seek;

        if (new_line &&
            new_line - bytes < seek)
        {
            write = new_line - bytes + 1;
        }

        write = fwrite(bytes,
                       sizeof(char),
                       write,
                       stdout);

        remain -= (int)write;
        bytes += write;
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

        path(full_path,
             entry->d_name);

        printf("\033[%d;%dH",
               j,
               left);

        if (i == dir->cursor)
        {
            enable_bold();
        }

        bool is_file = (!is_dir(entry)) ? true :
                                          false;

        const char *symbol = (is_file) ? ICON_FILE :
                                         ICON_FOLDER;


        if (!has_permissions(full_path,
                             R_OK))
        {
            symbol = ICON_DENY;
        }

        print_entry_info(entry->d_name,
                         symbol,
                         full_path,
                         left,
                         right,
                         j,
                         is_file,
                         is_preview);

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
    print_title(strrchr(pre->path,
                        '/') + 1,
                left,
                right);

    if (pre->type & PT_DIR)
    {
        print_directory(&pre->directory,
                        left,
                        right,
                        height,
                        true);
    }
    else
    {
        if (pre->type & (PT_INV | PT_EMP | PT_DEN))
        {
            set_text_colour(COLOUR_INFO);
        }

        print_file_contents(pre,
                            left,
                            right,
                            height);

        reset_formatting();
    }
}
