#include "sage.h"
#include "board.h"
#include "checker.h"
#include "dstats.h"

#define WINDOW_W (500)
#define WINDOW_H (1000)
#define MAX_ASPECT_RATIO (1.4)

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  Sage* s = Sage_create("Baackgammon!", WINDOW_W, WINDOW_H);
  int w, h, x, y;
  w = WINDOW_W * .9;
  h = w * MAX_ASPECT_RATIO;
  x = (WINDOW_W - w) * .5;
  y = (WINDOW_H - h) * .5;
  SDL_Rect normal = {x, y, w, h};
  Sage_setNormalRect(s, normal);
  Board_create(s);
  //Checker_create(s, 1, P_LIGHT, board);

  DStats_create(s);

  Sage_run(s);

  return 0;
}
