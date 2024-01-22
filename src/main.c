#include "sage.h"
#include "board.h"
#include "checker.h"
#include "dstats.h"
#include "log.h"

#define WINDOW_W (400)
#define WINDOW_H (800)
#define MAX_ASPECT_RATIO (1.4)
#define MIN_ASPECT_RATIO (.7)
#define MAX_BOARD_WIDTH_PCT (.9)

void findDimensions(int w, int h, int* x, int* y) {
  // Start from the maximum width which is 90% of w
  for (int possibleX = (int)(MAX_BOARD_WIDTH_PCT * w); possibleX > 0; possibleX--) {
    // For each width, find a height that maintains the aspect ratio
    for (int possibleY = h; possibleY > 0; possibleY--) {
      float aspectRatio = (float)possibleX / possibleY;
      // Check if the aspect ratio is within the range
      if (aspectRatio >= MIN_ASPECT_RATIO && aspectRatio <= MAX_ASPECT_RATIO) {
        *x = possibleX;
        *y = possibleY;
        return;
      }
    }
  }

  // If no suitable dimensions are found, return 0
  *x = 0;
  *y = 0;
}


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
