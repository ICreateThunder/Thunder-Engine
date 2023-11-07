#include <core/logger.h>
#include <core/asserts.h>

#include <core/application.h>

int main(void) {

    application_config config;
    config.start_x_pos = 100;
    config.start_y_pos = 100;
    config.start_width = 1280;
    config.start_height = 720;
    config.name = "Thunder's Engine Sandbox";

    application_create(&config);
    application_run();
    

    return 0;
}