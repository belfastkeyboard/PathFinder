#include <malloc.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "../file.h"
#include "../path.h"

#define  B (1)
#define KB (B * 1024)
#define MB (KB * 1024)
#define GB (MB * 1024)


int no_special(const struct dirent *entry)
{
    int result = strcmp(entry->d_name, ".") == 0 ||
                 strcmp(entry->d_name, "..") == 0;

    return !result;
}


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


static void load_directory(struct directory* dir)
{
    if (dir->list)
    {
        free(dir->list);
    }

    dir->nmemb = scandir(dir->path,
                         &dir->list,
                         no_special,
                         alphasort);

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


void init_path(char *current_dir,
               const char *path,
               struct directory *dir,
               struct preview *pre)
{
    strncpy(current_dir,
            realpath(path,
                    current_dir),
            PATH_MAX);

    dir->path = current_dir;

    step_out(dir,
             pre);
}

void load_preview(struct directory *dir,
                  struct preview *pre)
{
    static char preview_path[PATH_MAX] = { 0 };

    struct dirent *entry = dir->list[dir->cursor];

    strcpy(preview_path,
           dir->path);

    path(preview_path,
         entry->d_name);

    if (pre->is_dir)
    {
        free(pre->directory.list);
    }

    memset(pre,
           0,
           sizeof(struct preview));

    pre->path = preview_path;

    if (is_dir(entry))
    {
        pre->directory.path = pre->path;

        load_directory(&pre->directory);
        pre->is_dir = true;
    }
    else
    {
        struct filesize fs = file_size(pre->path);

        pre->file.bytes = malloc(fs.bytes);

        FILE *file = fopen(pre->path,
                           "r");

        fread(pre->file.bytes,
              sizeof(char),
              fs.bytes,
              file);

        pre->is_dir = false;

        fclose(file);
    }
}


void step_in(const char *new_name,
             struct directory *dir,
             struct preview *pre)
{
    dir->path = path(dir->path,
                     new_name);

    load_directory(dir);

    load_preview(dir,
                 pre);
}

void step_out(struct directory *dir,
              struct preview *pre)
{
    char *sep = strrchr(dir->path,
                        '/');

    if (sep != dir->path)
    {
        sep[0] = '\0';
    }
    else
    {
        sep[1] = '\0';
    }

    load_directory(dir);

    load_preview(dir,
                 pre);
}
