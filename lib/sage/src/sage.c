/**
 * @file sage.c
 * @author Michael Baumgarten
 * @brief Simply-Another-Game-Engine (SAGE)
 */
#include "sage.h"
#include <stdlib.h>

Sage sage = {0};

Sint32 delayFrame(Sint32 frameStart) {
    Sint32 frameTime = (Sint32)(SDL_GetTicks() - frameStart);
    if (frameTime < MS_PER_FRAME) {
        SDL_Delay(MS_PER_FRAME - frameTime);
    }
    return (Sint32)SDL_GetTicks();
}

void Sage_init(char* title, int winWidth, int winHeight, bool fillDisplay) {
    sage.mm = MM_init(title, winWidth, winHeight, fillDisplay);
    sage.vm = VM_init(sage.mm->renderer);
    sage.em = EM_init();
}

void Sage_destroy(void) {
    VM_free(sage.vm);
    MM_free(sage.mm);
}

void Sage_step(void) {
    VM_draw(sage.vm);
}

int Sage_handleEvent(SDL_Event* event) {
    EM_handleEvent(sage.em, event);
    if (event->type == SDL_EVENT_QUIT) {
        return 1;
    }
    return 0;
}

void Sage_setViewport(SDL_FRect viewport) {
    VM_setViewport(sage.vm, viewport);
}

SDL_Texture* Sage_loadTexture(char* path) {
    return MM_loadTexture(sage.mm, path);
}

SDL_Texture* Sage_loadSVGTexture(char* path, Sint32 width, Sint32 height) {
    return MM_loadSVGTexture(sage.mm, path, width, height);
}

float Sage_convertHighDPI(float value) {
    return sage.mm->pixelScale * value;
}

TTF_Font* Sage_loadFont(char* path, Sint32 size) {
    return MM_loadFont(sage.mm, path, size);
}

Sint32 Sage_registerSprite(Sprite* sprite) {
    return VM_registerSprite(sage.vm, sprite);
}

void Sage_registerSnippet(Snippet* snippet) {
    VM_registerSnippet(sage.vm, snippet);
}

void Sage_registerEventCallback(Uint32 eventType, EventCallback_fn callback_fn, void* data) {
    EM_registerCallback(sage.em, eventType, callback_fn, data);
}

void Sprite_setZ(Sprite* s, Sint32 newZ){
    VM_setSpriteZ(sage.vm, s, newZ);
}
