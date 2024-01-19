#include "sage.h"
#include "board.h"
#include "checker.h"
#include "dstats.h"

#define WINDOW_W (1920)
#define WINDOW_H (1080)

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  Sage* s = Sage_create("Baackgammon!", WINDOW_W, WINDOW_H);
  SDL_Rect normal = { 0, 0, WINDOW_W, WINDOW_H };
  Sage_setNormalRect(s, normal);
  Board_create(s);
  //Checker_create(s, 1, P_LIGHT, board);

  //DStats_create(s);

  Sage_run(s);

  return 0;
}
