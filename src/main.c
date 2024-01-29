#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <stdlib.h>

#include "sage.h"
#include "util.h"
#include "checker.h"
#include "dstats.h"
#include "log.h"
#include "game.h"

int SDL_AppInit(int argc, char **argv){
  (void)argc;
  (void)argv;
  extern Sage sage;
  Sage_init("Backgammon!", WINDOW_W, WINDOW_H, false);
  srand(0);
  GameBoard_create();
  return 0;
}

int SDL_AppIterate(void){
  Sage_step();
  return 0;
}

int SDL_AppEvent(const SDL_Event *event){
  SDL_Event e = *event;
  return Sage_handleEvent(&e);
}

void SDL_AppQuit(void){
  Sage_destroy();
}
