#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "../file.h"
#include "../path.h"
#include "../settings.h"


#define  B (1)
#define KB (B * 1024)
#define MB (KB * 1024)
#define GB (MB * 1024)

typedef int (*SortFunc)(const struct dirent **, const struct dirent **);


static int sort_alphabetical(const struct dirent **entry1,
                             const struct dirent **entry2)
{
    const char *a = (*entry1)->d_name;
    const char *b = (*entry2)->d_name;

    while (*a == *b)
    {
        a++;
        b++;
    }

    const int x = tolower(*a);
    const int y = tolower(*b);

    return ((x > y) - (x < y));
}

static int sort_type(const struct dirent **entry1,
                     const struct dirent **entry2)
{
    const unsigned char x = (*entry1)->d_type;
    const unsigned char y = (*entry2)->d_type;

    return ((x > y) - (x < y));
}


static SortFunc sort_table[] =
{
    sort_alphabetical,
    sort_type
};

int sort_index = 0;
const int sort_table_max = sizeof(sort_table) / sizeof(SortFunc);


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


void load_directory(struct directory* dir,
                    const settings settings)
{
    if (dir->list)
    {
        free(dir->list);
    }

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
               const settings settings)
{
    realpath(path,
             current_dir);

    pre->type = PT_DEN;

    dir->path = current_dir;

    step_out(dir,
             pre,
             settings);
}

void load_preview(struct directory *dir,
                  struct preview *pre,
                  const settings settings)
{
    static char preview_path[PATH_MAX] = { 0 };
    struct dirent *entry = dir->list[dir->cursor];

    strcpy(preview_path,
           dir->path);

    path(preview_path,
         entry->d_name);

    if (pre->type & PT_DIR)
    {
        free(pre->directory.list);
    }
    else if (pre->type & PT_FIL)
    {
        free(pre->file.bytes);
    }

    memset(pre,
           0,
           sizeof(struct preview));

    pre->path = preview_path;

    if (has_permissions(pre->path,
                        R_OK))
    {
        if (is_dir(entry))
        {
            pre->directory.path = pre->path;

            load_directory(&pre->directory,
                           settings);

            pre->type = PT_DIR;
        }
        else
        {
            struct filesize fs = file_size(pre->path);

            pre->file.count = (int)fs.bytes;
            pre->file.bytes = malloc(fs.bytes);

            FILE *file = fopen(pre->path,
                               "r");

            fread(pre->file.bytes,
                  sizeof(char),
                  fs.bytes,
                  file);

            pre->type = PT_FIL;

            fclose(file);
        }
    }
    else
    {
        pre->type = PT_DEN;

        strcpy(pre->denied.msg,
               "You do not have permission to preview this content.");
    }
}


void step_in(const char *new_name,
             struct directory *dir,
             struct preview *pre,
             const settings settings)
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
                     settings);
    }
    else
    {
        // TODO: display error message

        get_parent(dir->path);
    }
}

void step_out(struct directory *dir,
              struct preview *pre,
              const settings settings)
{
    get_parent(dir->path);

    if (has_permissions(dir->path,
                        R_OK))
    {
        load_directory(dir,
                       settings);

        load_preview(dir,
                     pre,
                     settings);
    }
    else
    {
        const size_t len = strlen(dir->path);

        dir->path[len] = '/';
    }
}
