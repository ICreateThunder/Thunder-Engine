#include <core/logger.h>
#include <core/asserts.h>
#include <platform/platform.h>

int main(void) {
    KFATAL("Something went wrong: %f", 3.14f);
    KERROR("Something went wrong: %f", 3.14f)
    KWARN("Something went wrong: %f", 3.14f);
    KINFO("Something went wrong: %f", 3.14f);
    KDEBUG("Something went wrong: %f", 3.14f);
    KTRACE("Something went wrong: %f", 3.14f);

    platform_state state;
    if (platform_startup(&state, "Thunder Engine Sandbox", 100, 100, 1280, 720)) {
        while(TRUE) {
            platform_pump_messages(&state);
        }
    }
    platform_shutdown(&state);

    return 0;
}