#include "sage.h"
#include "board.h"
#include "checker.h"
#include "dstats.h"

#define WINDOW_W (1920)
#define WINDOW_H (1080)
#define MAX_ASPECT_RATIO (1.4)
#define MIN_ASPECT_RATIO (.7)

void findDimensions(int w, int h, int* x, int* y) {
  // Start from the maximum width which is 90% of w
  for (int possibleX = (int)(0.9 * w); possibleX > 0; possibleX--) {
    // For each width, find a height that maintains the aspect ratio
    for (int possibleY = h; possibleY > 0; possibleY--) {
      float aspectRatio = (float)possibleX / possibleY;
      // Check if the aspect ratio is within the range
      if (aspectRatio >= 0.7 && aspectRatio <= 1.4) {
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

  SDL_Rect normal = { x, y, w, h };
  Sage_setNormalRect(s, normal);
  Board_create(s);
  // for (int i = 1; i <= 24; i++) {
  //   Player p = i % 2 == 1 ? P_LIGHT : P_DARK;
  //   Checker_create(s, i, p);
  // }
  Checker_create(s, 1, P_LIGHT);

  DStats_create(s);

  Sage_run(s);

  return 0;
}
