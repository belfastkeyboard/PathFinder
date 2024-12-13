#include "../file.h"
#include "../input.h"
#include "../screen.h"


// TODO:
//  display error message in red text on the bottom line
//  scroll dir previews
//  open files with 'o' in associated application
//  open thunar by pressing 'd'
//  copy-paste files
//  only read enough bytes to display on the preview
//  scrolling file previews are a bit banjaxed
//  drag-n-drop preview separator
//  delete files with remove(), directories with rmdir()


int main(int argc, char *argv[]) {
    enable_raw_mode();

    char current_path[PATH_MAX] = {0};
    struct directory directory = {0};
    struct preview preview = {0};
    struct screen screen = {0};
    key key = {0};
    settings settings = SETTINGS_PREV;

    refresh_screen(&screen,
                   settings);

    init_path(current_path,
              argv[0],
              &directory,
              &preview,
              settings,
              screen.preview_divider,
              screen.width,
              screen.height);

    bool exit = false;

    while (!exit)
    {
        new_screen();

        print_directory(&directory,
                        0,
                        screen.directory_right,
                        screen.height,
                        false);

        if (settings & SETTINGS_PREV)
        {
            print_vertical_line(screen.preview_divider,
                                screen.height);

            print_preview(&preview,
                          screen.preview_divider + 1,
                          screen.width,
                          screen.height);
        }

        get_input(key);

        if (change_settings(&settings,
                            key))
        {
            load_directory(&directory,
                           settings);

            load_preview(&directory,
                         &preview,
                         settings,
                         screen.preview_divider,
                         screen.width,
                         screen.height);
        }

        move_cursor(key,
                    &directory,
                    &preview,
                    settings,
                    screen.preview_divider,
                    screen.width,
                    screen.height);

        move_dir(key,
                 &directory,
                 &preview,
                 settings,
                 screen.preview_divider,
                 screen.width,
                 screen.height);

        scroll_file(key,
                    &preview,
                    settings,
                    screen.preview_divider,
                    screen.width,
                    screen.height);


        refresh_screen(&screen,
                       settings);

        exit |= (open_file(key,
                           &directory) |
                 should_exit(key));
    }

    unload_directory(&directory);
    unload_preview(&preview);
    disable_raw_mode();
    close_screen();
}
