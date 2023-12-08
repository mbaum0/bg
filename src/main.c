
#include <SDL.h>
#include <stdio.h>
#include "game.h"

// #define FPS 50
// #define MS_PER_FRAME (1000 / FPS)

// void update_spr(Sprite* sprite, void* data) {
//     (void)data;
//     // move the sprite
//     Sprite_setLocation(sprite, Sprite_getX(sprite) + 1, Sprite_getY(sprite) + 1);
// }


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    GameManager* gm = GM_init();
    GM_run(gm);
    GM_free(gm);
}
