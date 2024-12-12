#include "../settings.h"

#define NO_RELOAD 0
#define RELOAD    1


extern int sort_index;
extern const int sort_table_max;


int change_settings(settings *settings,
                    const key key)
{
    int result = NO_RELOAD;
    char key_press = key[0];

    if (key_press == 'h')
    {
        *settings ^= SETTINGS_HIDE;
        result = RELOAD;
    }
    else if (key_press == 'p')
    {
        *settings ^= SETTINGS_PREV;
    }
    else if (key_press == 's')
    {
        sort_index = (sort_index + 1) % sort_table_max;
        result = RELOAD;
    }

    return result;
}
