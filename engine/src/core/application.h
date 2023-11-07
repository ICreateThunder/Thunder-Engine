#pragma once

#include "defines.h"

typedef struct application_config {
    i16 start_x_pos;
    i16 start_y_pos;
    i16 start_width;
    i16 start_height;
    char* name;
} application_config;

KAPI b8 application_create(application_config* config);

KAPI b8 application_run();