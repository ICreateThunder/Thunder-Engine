#include "application.h"

#include "logger.h"
#include "platform/platform.h"

typedef struct application_state {
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialised = FALSE;
static application_state app_state;

b8 application_create(application_config* config) {
    if (initialised) {
        KERROR("application_create called more than once!");
        return FALSE;
    }

    // Initialise subsystems
    initialse_logging();

    // TODO: Remove after testing/adding more functionality
    KFATAL("Something went wrong: %f", 3.14f);
    KERROR("Something went wrong: %f", 3.14f)
    KWARN("Something went wrong: %f", 3.14f);
    KINFO("Something went wrong: %f", 3.14f);
    KDEBUG("Something went wrong: %f", 3.14f);
    KTRACE("Something went wrong: %f", 3.14f);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if (!platform_startup(&app_state.platform, config->name, config->start_x_pos, config->start_y_pos, config->start_width, config->start_height)) {
        return FALSE;
    } 

    initialised = TRUE;

    return TRUE;
}

b8 application_run() {
    while(app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }
    }

    app_state.is_running = FALSE;

    platform_shutdown(&app_state.platform);

    return TRUE;
}