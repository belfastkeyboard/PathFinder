#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <dirent.h>
#include "types.h"


#define PT_NUL  (1 << 0)
#define PT_DIR  (1 << 1)
#define PT_FIL  (1 << 2)
#define PT_DEN  (1 << 3)
#define PT_EMP  (1 << 4)
#define PT_INV  (1 << 5)


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
    size_t read;
    size_t size;
    long offset;
};


struct preview
{
    char type;
    char *path;
    union {
        struct directory directory;
        struct file file;
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
               struct preview *pre,
               settings settings,
               int left,
               int right,
               int height);


void load_directory(struct directory* dir,
                    settings settings);

void load_preview(struct directory *dir,
                  struct preview *pre,
                  settings settings,
                  int left,
                  int right,
                  int height);

void load_file(struct preview *pre,
               size_t count,
               int left,
               int right,
               int height);

void unload_directory(struct directory *dir);

void unload_preview(struct preview *pre);


void step_in(const char *new_name,
             struct directory *dir,
             struct preview *pre,
             settings settings,
             int left,
             int right,
             int height);

void step_out(struct directory *dir,
              struct preview *pre,
              settings settings,
              int left,
              int right,
              int height);
