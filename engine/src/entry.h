#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "game_types.h"

extern b8 create_game(game* out_game);

int main(void) {
    

    game game_inst;

    // Attempt to create game
    if (!create_game(&game_inst)) {
        KFATAL("Could not create game");
        return -1;
    }

    // Ensure function pointers exist
    if (!game_inst.render || !game_inst.update || !game_inst.initialise || !game_inst.on_resize) {
        KFATAL("The game's function pointers must be assigned");
        return -2;
    }

    // Application initilisation
    if (!application_create(&game_inst)) {
        KERROR("Application failed to create!");
        return 1;
    }

    // Begin the game loop
    if (!application_run()) {
        KERROR("Application did not shutdown gracefully!");
        return 2;
    }

    

    return 0;
}