
#include <SDL.h>
#include <stdio.h>
#include "media.h"
#include "view.h"

void update_spr(Sprite* sprite, void* data) {
    (void)data;
    // move the sprite
    Sprite_setLocation(sprite, Sprite_getX(sprite) + 1, Sprite_getY(sprite) + 1);
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    MediaManager* mediaManager = MM_init();
    ViewManager* viewManager = VM_init(mediaManager->renderer);

    SDL_Rect src = {0, 0, 60, 60};
    VM_createSprite(viewManager, mediaManager->textures.dice, src, 0, 0, update_spr, NULL);

    for(uint32_t i = 0; i < 100; i++) {
        VM_draw(viewManager);
        SDL_Delay(10);
    }

    VM_free(viewManager);
    MM_free(mediaManager);
}
