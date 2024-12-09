#pragma once

#include "file.h"


void new_screen(void);

void close_screen(void);


void enable_raw_mode(void);

void disable_raw_mode(void);


void enable_bold(void);

void reset_formatting(void);


void print_directory(const struct entries *entries);
