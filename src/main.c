#include "../file.h"
#include "../input.h"
#include "../screen.h"



// TODO:
//  get current directory
//  display contents
//  navigate files

int main(void)
{
    enable_raw_mode();

    char current_dir[MAX_DIR_LEN] = { 0 };
    struct entries entries = { 0 };
    char key[KEY_LEN] = { 0 };

    change_dir(current_dir,
               ".",
               &entries);

    while (true)
    {
        new_screen();

        print_directory(&entries);

        get_input(key);

        if (should_exit(key))
        {
            break;
        }

        move_cursor(key,
                    &entries);
    }

    disable_raw_mode();

    close_screen();

    return 0;
}
