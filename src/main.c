#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "sage.h"
#include "board.h"
#include "util.h"
#include "checker.h"
#include "dstats.h"
#include "log.h"
#include "pip.h"

int SDL_AppInit(int argc, char **argv){
  (void)argc;
  (void)argv;
  extern Sage sage;
  Sage_init("Backgammon!", WINDOW_W, WINDOW_H, false);
  Board_create();
  Pips_create();
  GameBoard_create();
  //DStats_create();
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
