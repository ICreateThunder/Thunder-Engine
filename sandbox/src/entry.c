#include "game.h"

#include <entry.h>

#include <core/kmemory.h>

b8 create_game(game* out_game) {
    // Application configuration
    out_game->app_config.start_x_pos = 100;
    out_game->app_config.start_y_pos = 100;
    out_game->app_config.start_width = 1280;
    out_game->app_config.start_height = 720;
    out_game->app_config.name = "Thunder's Engine Sandbox"; 
    out_game->update = game_update;
    out_game->render = game_render;
    out_game->initialise = game_initialise;
    out_game->on_resize = game_on_resize;

    out_game->state = kallocate(sizeof(game_state), MEMORY_TAG_GAME);
    
    return TRUE;
}