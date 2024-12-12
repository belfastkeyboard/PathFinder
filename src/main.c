#include "../file.h"
#include "../input.h"
#include "../screen.h"
#include "../settings.h"


// TODO:
//  file previews have weird buggy trail at the end
//  cannot display file message is bugged
//  scrolling file previews
//  display error message in red text on the bottom line


int main(int argc, char *argv[])
{
    enable_raw_mode();

    char current_path[PATH_MAX] = { 0 };
    struct directory directory = { 0 };
    struct preview preview = { 0 };
    key key = { 0 };
    settings settings = SETTINGS_PREV;

    init_path(current_path,
              argv[0],
              &directory,
              &preview,
              settings);

    while (true)
    {
        const int w = screen_width();
        const int h = screen_height();

        const int pre_div = (int)(w * 0.65);

        new_screen();

        const int dir_r = (settings & SETTINGS_PREV) ? pre_div :
                                                       w;

        print_directory(&directory,
                        0,
                        dir_r,
                        h,
                        false);


        if (settings & SETTINGS_PREV)
        {
            print_vertical_line(pre_div,
                                h);

            print_preview(&preview,
                          pre_div,
                          w,
                          h);
        }

        get_input(key);

        if (should_exit(key))
        {
            break;
        }
        else if (change_settings(&settings,
                                 key))
        {
            load_directory(&directory,
                           settings);

            load_preview(&directory,
                         &preview,
                         settings);
        }

        move_cursor(key,
                    &directory,
                    &preview,
                    settings,
                    h);

        move_dir(key,
                 &directory,
                 &preview,
                 settings);
    }

    disable_raw_mode();

    close_screen();

    return 0;
}
