
#include <SDL.h>
#include <stdio.h>
#include "media.h"
#include "view.h"

#define FPS 50
#define MS_PER_FRAME (1000 / FPS)

void update_spr(Sprite* sprite, void* data) {
    (void)data;
    // move the sprite
    Sprite_setLocation(sprite, Sprite_getX(sprite) + 1, Sprite_getY(sprite) + 1);
}

bool processInput(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            log_debug("Received SDL_QUIT event");
            return true;
        default:
            break;
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    MediaManager* mediaManager = MM_init();
    ViewManager* viewManager = VM_init(mediaManager->renderer);

    SDL_Rect src = {0, 0, 60, 60};
    VM_createSprite(viewManager, mediaManager->textures.dice, src, 0, 0, update_spr, NULL);
    uint32_t frameStart, frameTime;
    while(!processInput()){
        frameStart = SDL_GetTicks();
        VM_draw(viewManager);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < MS_PER_FRAME) {
            SDL_Delay(MS_PER_FRAME - frameTime);
        }
    }

    VM_free(viewManager);
    MM_free(mediaManager);
}
