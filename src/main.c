#include "sage.h"
#include "board.h"

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  Sage* s = Sage_create("Baackgammon!", 375, 812);
  Board_create(s, 375, 812);

  Sage_run(s);

  return 0;
}
