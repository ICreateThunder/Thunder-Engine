#include "application.h"
#include "game_types.h"

#include "logger.h"
#include "platform/platform.h"
#include "core/kmemory.h"
#include "core/event.h"

typedef struct application_state {
    game* game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialised = FALSE;
static application_state app_state;

b8 application_create(game* game_inst) {
    if (initialised) {
        KERROR("application_create called more than once!");
        return FALSE;
    }

    app_state.game_inst = game_inst;

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

    if (!event_initialise()) {
        KFATAL("Event system failed to initialise. Application cannot continue.");
        return FALSE;
    }

    if (!platform_startup(&app_state.platform, game_inst->app_config.name,game_inst->app_config.start_x_pos, game_inst->app_config.start_y_pos, game_inst->app_config.start_width, game_inst->app_config.start_height)) {
        return FALSE;
    } 

    if (!app_state.game_inst->initialise(app_state.game_inst)) {
        KFATAL("Game failed to initilaise");
        return FALSE;
    }

    app_state.game_inst->on_resize(app_state.game_inst, app_state.width, app_state.height);

    initialised = TRUE;

    return TRUE;
}

b8 application_run() {
    // TODO: Sort this code
    void* memory_status = get_memory_usage_str();
    KINFO(memory_status);
    kfree(memory_status, 0, MEMORY_TAG_UNKNOWN);

    while(app_state.is_running) {
        if (!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended) {
            if (!app_state.game_inst->update(app_state.game_inst, (f32)0)) {
                KFATAL("Game update failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }

            if (!app_state.game_inst->render(app_state.game_inst, (f32)0)) {
                KFATAL("Game render failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }
        }
    }

    app_state.is_running = FALSE;

    event_shutdown();

    platform_shutdown(&app_state.platform);

    return TRUE;
}