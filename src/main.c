#include "ball.h"
#include "paddle.h"
#include "sage.h"
#include "sand.h"

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  Sage* s = Sage_create("SandSim", 800, 600);
  // Ball* b = Ball_create(s, 4, 4);
  // Paddle* p = Paddle_create(s);
  Sand_init(s);


  Sage_run(s);

  return 0;
}
