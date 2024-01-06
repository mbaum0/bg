#include "sage.h"
#include "ball.h"
#include "paddle.h"

int main(int argc, char** argv){
    (void)argc;
    (void)argv;
    Sage* s = Sage_create("PaddleBall", 800, 600);
    Ball* b = Ball_create(s, 4, 4);
    Paddle* p = Paddle_create(s);

    Sage_run(s);
    Ball_destroy(b);
    Paddle_destroy(p);
    Sage_destroy(s);
    return 0;
}
