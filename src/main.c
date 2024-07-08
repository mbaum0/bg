#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <stdlib.h>

#include "checker.h"
#include "dstats.h"
#include "fsm.h"
#include "game.h"
#include "log.h"
#include "sage.h"
#include "util.h"

int SDL_AppInit(void** appstate, int argc, char** argv) {
    (void)argc;
    (void)argv;
    (void)appstate;
    extern Sage sage;
    Sage_init("Backgammon!", WINDOW_W, WINDOW_H);
    srand(7);
    fsm_init();
    gameboard_init();
    // DStats_create();
    return 0;
}

int SDL_AppIterate(void* appstate) {
    (void)appstate;
    Sage_step();
    fsm_step();
    return 0;
}

int SDL_AppEvent(void* appstate, const SDL_Event* event) {
    (void)appstate;
    SDL_Event e = *event;
    return Sage_handleEvent(&e);
}

void SDL_AppQuit(void* appstate) {
    (void)appstate;
    Sage_destroy();
}
