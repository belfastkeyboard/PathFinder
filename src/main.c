#include "../file.h"
#include "../input.h"
#include "../screen.h"


// TODO:
//  settings (just put it on a bitmask)
//  'h': hide hidden files (filter out entry->d_name[0] == '.')
//  'p': do not display preview
//  's': cycle through sorting options
//  display error message in red text on the bottom line

#include <stdio.h>


int main(int argc, char *argv[])
{
    FILE *file = fopen("log.txt", "w");
    fclose(file);

    enable_raw_mode();

    char current_path[PATH_MAX] = { 0 };

    struct directory directory = { 0 };
    struct preview preview = { 0 };
    char key[KEY_LEN] = { 0 };

    init_path(current_path,
              argv[0],
              &directory,
              &preview);

    while (true)
    {
        const int w = screen_width();
        const int h = screen_height();

        const int pre_div = (int)(w * 0.65);

        new_screen();

        print_directory(&directory,
                        0,
                        pre_div,
                        h,
                        false);

        print_vertical_line(pre_div,
                            h);

        print_preview(&preview,
                      pre_div,
                      w,
                      h);

        get_input(key);

        if (should_exit(key))
        {
            break;
        }

        move_cursor(key,
                    &directory,
                    &preview,
                    h);

        move_dir(key,
                 &directory,
                 &preview);
    }

    disable_raw_mode();

    close_screen();

    return 0;
}
