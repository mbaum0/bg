#include "sage.h"
#include "board.h"
#include "util.h"
#include "checker.h"
#include "dstats.h"
#include "log.h"
#include <SDL3/SDL_main.h>

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  Sage* s = Sage_create("Baackgammon!", WINDOW_W, WINDOW_H);

  int w, h, x, y;
  findDimensions(WINDOW_W, WINDOW_H, &w, &h);

  x = (WINDOW_W - w) * .5;
  y = (WINDOW_H - h) * .5;

  SDL_FRect normal = { x, y, w, h };
  Sage_setNormalRect(s, normal);
  extern DStats stats;
  stats.gameWidth = w;
  stats.gameHeight = h;
  Board_create(s);
  // for (int i = 1; i <= 24; i++) {
  //   Player p = i % 2 == 1 ? P_LIGHT : P_DARK;
  //   Checker_create(s, i, p);
  // }
  //Checker_create(s, 1, P_LIGHT);
  GameBoard* gb = GameBoard_create(s);

  DStats_create(s);

  Sage_run(s);
  GameBoard_destroy(gb);

  return 0;
}
