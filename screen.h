#pragma once

#include "file.h"
#include "settings.h"


struct screen
{
    int width;
    int height;
    int preview_divider;
    int directory_right;
};


void new_screen(void);

void close_screen(void);

void refresh_screen(struct screen *screen,
                    settings settings);


int screen_width(void);

int screen_height(void);


void enable_raw_mode(void);

void disable_raw_mode(void);


void print_vertical_line(int x,
                         int height);


void print_directory(const struct directory *dir,
                     int left,
                     int right,
                     int height,
                     bool is_preview);

void print_preview(const struct preview *pre,
                   int left,
                   int right,
                   int height);
