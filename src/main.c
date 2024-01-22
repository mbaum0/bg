#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "sage.h"
#include "board.h"
#include "util.h"
#include "checker.h"
#include "dstats.h"
#include "log.h"

int SDL_AppInit(int argc, char **argv){
  (void)argc;
  (void)argv;
  extern Sage sage;
  extern DStats stats;
  Sage_init("Backgammon!", WINDOW_W, WINDOW_H);

  int w, h, x, y;
  findDimensions(WINDOW_W, WINDOW_H, &w, &h);
  x = (WINDOW_W - w) * .5;
  y = (WINDOW_H - h) * .5;
  SDL_FRect normal = { x, y, w, h };
  Sage_setNormalRect(normal);
  stats.gameWidth = w;
  stats.gameHeight = h;
  Board_create();
  GameBoard_create();
  DStats_create();
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

// int main(int argc, char** argv) {
//   Board_create(s);
//   // for (int i = 1; i <= 24; i++) {
//   //   Player p = i % 2 == 1 ? P_LIGHT : P_DARK;
//   //   Checker_create(s, i, p);
//   // }
//   //Checker_create(s, 1, P_LIGHT);
//   GameBoard* gb = GameBoard_create(s);

//   DStats_create(s);

//   Sage_run(s);
//   GameBoard_destroy(gb);

//   return 0;
// }
