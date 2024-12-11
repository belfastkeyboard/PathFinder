#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <dirent.h>

#define PT_DIR  (1 << 0)
#define PT_FIL  (1 << 1)
#define PT_DEN  (1 << 2)


struct filesize
{
    char suffix[16];
    int len;
    long bytes;
};

struct directory
{
    char *title;
    char *path;
    struct dirent **list;
    int nmemb;
    int cursor;
    int offset;
};

struct file
{
    char *bytes;
    int count;
};

struct denied
{
    char msg[64];
};


struct preview
{
    char type;
    char *path;
    union {
        struct directory directory;
        struct file file;
        struct denied denied;
    };
};


int no_special(const struct dirent *entry);


struct filesize file_size(const char *path);


bool is_dir(const struct dirent *entry);

bool is_root(const char *path);

bool has_permissions(const char *path,
                     int mode);


void init_path(char *current_dir,
               const char *path,
               struct directory *dir,
               struct preview *pre);

void load_preview(struct directory *dir,
                  struct preview *pre);


void step_in(const char *new_name,
             struct directory *dir,
             struct preview *pre);

void step_out(struct directory *dir,
              struct preview *pre);
