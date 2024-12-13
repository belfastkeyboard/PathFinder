#include <malloc.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "../ext.h"
#include "../file.h"
#include "../path.h"
#include "../settings.h"
#include "../sort.h"


#define  B (1)
#define KB (B * 1024)
#define MB (KB * 1024)
#define GB (MB * 1024)


static SortFunc sort_table[] =
{
        sort_alphabetical,
        sort_type
};

int sort_index = 0;
const int sort_table_max = sizeof(sort_table) / sizeof(SortFunc);


static struct filesize get_size_as_string(const long sz)
{
    struct filesize size = { 0 };

    float factor;
    char *suffix;

    if (sz > KB)
    {
        if (sz > GB)
        {
            factor = GB;
            suffix = "Gb";
        }
        else if (sz > MB)
        {
            factor = MB;
            suffix = "Mb";
        }
        else
        {
            factor = KB;
            suffix = "Kb";
        }

        snprintf(size.suffix, 16, "%.2f%s",
                 (float)sz / factor,
                 suffix);
    }
    else
    {
        suffix = "b ";

        snprintf(size.suffix, 16, "%ld%s",
                 sz,
                 suffix);
    }

    size.len = (int)(strlen(size.suffix));
    size.bytes = sz;

    return size;
}

static void get_parent(char *path)
{
    char *sep = strrchr(path,
                        '/');

    if (sep != path)
    {
        sep[0] = '\0';
    }
    else
    {
        sep[1] = '\0';
    }
}


int no_special(const struct dirent *entry)
{
    int result = strcmp(entry->d_name, ".") == 0 ||
                 strcmp(entry->d_name, "..") == 0;

    return !result;
}

int hide_hidden(const struct dirent *entry)
{
    return entry->d_name[0] != '.';
}


void load_directory(struct directory* dir,
                    const settings settings)
{
    unload_directory(dir);

    int (*filter)(const struct dirent *) = (settings & SETTINGS_HIDE) ? hide_hidden :
                                                                        no_special;

    int (*compare)(const struct dirent **,
                   const struct dirent **) = sort_table[sort_index];

    dir->nmemb = scandir(dir->path,
                         &dir->list,
                         filter,
                         compare);

    dir->cursor = 0;
    dir->offset = 0;

    if (is_root(dir->path))
    {
        dir->title = dir->path;
    }
    else
    {
        dir->title = strrchr(dir->path,
                             '/') + 1;
    }

    if (dir->nmemb == -1)
    {
        perror("scandir");

        exit(1);
    }
}


struct filesize file_size(const char *path)
{
    struct stat file;

    int res = stat(path,
                   &file);

    if (res == -1)
    {
        fprintf(stderr, "%s\n", path);
        perror("stat");

        exit(1);
    }

    return get_size_as_string(file.st_size);
}

bool is_dir(const struct dirent *entry)
{
    return entry->d_type & DT_DIR;
}

bool is_root(const char *path)
{
    return strcmp(path, "/") == 0;
}

bool has_permissions(const char *path,
                     const int mode)
{
    return !access(path,
                   mode);
}


void init_path(char *current_dir,
               const char *path,
               struct directory *dir,
               struct preview *pre,
               const settings settings,
               const int left,
               const int right,
               const int height)
{
    realpath(path,
             current_dir);

    pre->type = PT_NUL;

    dir->path = current_dir;

    step_out(dir,
             pre,
             settings,
             left,
             right,
             height);
}

void load_preview(struct directory *dir,
                  struct preview *pre,
                  const settings settings,
                  const int left,
                  const int right,
                  const int height)
{
    static char preview_path[PATH_MAX] = { 0 };
    struct dirent *entry = dir->list[dir->cursor];

    strcpy(preview_path,
           dir->path);

    path(preview_path,
         entry->d_name);

    unload_preview(pre);

    pre->path = preview_path;

    bool is_directory = is_dir(entry);
    bool is_permitted = has_permissions(pre->path,
                                        R_OK);

    if (is_permitted &&
        is_directory)
    {
        pre->directory.path = pre->path;

        load_directory(&pre->directory,
                       settings);

        pre->type = PT_DIR;
    }
    else
    {
        bool valid = is_file_valid(pre->path);
        struct filesize fs = file_size(pre->path);
        const size_t count = fs.bytes;

        pre->file.offset = 0;

        if (is_permitted &&
            valid &&
            count)
        {
            pre->type = PT_FIL;

            load_file(pre,
                      count,
                      left,
                      right,
                      height);
        }
        else
        {
            const char *message;

            if (!is_permitted)
            {
                pre->type = PT_DEN;
                message = "ⓘ You do not have permission to preview this content.";
            }
            else if (!count)
            {
                pre->type = PT_EMP;
                message = "ⓘ This file is empty.";
            }
            else
            {
                pre->type = PT_INV;
                message = "ⓘ This filetype cannot be previewed.";
            }

            pre->file.size = strlen(message);
            pre->file.bytes = malloc(pre->file.size + 1);

            strncpy(pre->file.bytes,
                    message,
                    pre->file.size + 1);

            pre->file.read = pre->file.size;
        }
    }
}

void load_file(struct preview *pre,
               const size_t count,
               const int left,
               const int right,
               const int height)
{
    const int space = (right - left) * height;
    const size_t to_read = (count < space) ? count :
                                             space;

    pre->file.size = count;
    pre->file.bytes = malloc(to_read);

    FILE *file = fopen(pre->path,
                       "r");

    fseek(file,
          pre->file.offset,
          SEEK_SET);

    pre->file.read = fread(pre->file.bytes,
                           sizeof(char),
                           to_read,
                           file);

    fclose(file);
}

void unload_directory(struct directory *dir)
{
    if (dir->list)
    {
        for (int i = 0; i < dir->nmemb; i++)
        {
            free(dir->list[i]);
        }

        free(dir->list);
        dir->list = NULL;
    }
}

void unload_preview(struct preview *pre)
{
    if (pre->type & PT_DIR)
    {
        unload_directory(&pre->directory);
    }
    else if (pre->type != PT_NUL)
    {
        free(pre->file.bytes);
    }

    memset(pre,
           0,
           sizeof(struct preview));
}


void step_in(const char *new_name,
             struct directory *dir,
             struct preview *pre,
             const settings settings,
             const int left,
             const int right,
             const int height)
{
    dir->path = path(dir->path,
                     new_name);

    if (has_permissions(dir->path,
                        R_OK))
    {
        load_directory(dir,
                       settings);

        load_preview(dir,
                     pre,
                     settings,
                     left,
                     right,
                     height);
    }
    else
    {
        // TODO: display error message

        get_parent(dir->path);
    }
}

void step_out(struct directory *dir,
              struct preview *pre,
              const settings settings,
              const int left,
              const int right,
              const int height)
{
    get_parent(dir->path);

    if (has_permissions(dir->path,
                        R_OK))
    {
        load_directory(dir,
                       settings);

        load_preview(dir,
                     pre,
                     settings,
                     left,
                     right,
                     height);
    }
    else
    {
        const size_t len = strlen(dir->path);

        dir->path[len] = '/';
    }
}
