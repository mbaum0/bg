#include "sage.h"
#include "board.h"

#define WINDOW_W (1920)
#define WINDOW_H (1080)

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  Sage* s = Sage_create("Baackgammon!", WINDOW_W, WINDOW_H);
  Board_create(s, WINDOW_W, WINDOW_H);

  Sage_run(s);

  return 0;
}
