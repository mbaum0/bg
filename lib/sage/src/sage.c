/**
 * @file sage.c
 * @author Michael Baumgarten
 * @brief Simply-Another-Game-Engine (SAGE)
 */
#include <stdlib.h>
#include "sage.h"

Sage sage = { 0 };

int32_t delayFrame(int32_t frameStart) {
  int32_t frameTime = SDL_GetTicks() - frameStart;
  if (frameTime < MS_PER_FRAME) {
    SDL_Delay(MS_PER_FRAME - frameTime);
  }
  return SDL_GetTicks();
}

void Sage_init(char* title, int winWidth, int winHeight){
  sage.mm = MM_init(title, winWidth, winHeight);
  sage.vm = VM_init(sage.mm->renderer);
  sage.em = EM_init();
}

void Sage_destroy(void) {
  VM_free(sage.vm);
  MM_free(sage.mm);
}

void Sage_step(void){
  VM_draw(sage.vm);
}

int Sage_handleEvent(SDL_Event *event){
  EM_handleEvent(sage.em, event);
  if (event->type == SDL_EVENT_QUIT){
    return 1;
  }
  return 0;
}

void Sage_setViewport(SDL_FRect viewport){
  VM_setViewport(sage.vm, viewport);
}

SDL_Texture* Sage_loadTexture(char* path) {
  return MM_loadTexture(sage.mm, path);
}

SDL_Texture* Sage_loadSVGTexture(char* path, int32_t width, int32_t height){
  return MM_loadSVGTexture(sage.mm, path, width, height);
}

TTF_Font* Sage_loadFont(char* path, int32_t size) {
  return MM_loadFont(sage.mm, path, size);
}

int32_t Sage_registerSprite(Sprite* sprite) {
  return VM_registerSprite(sage.vm, sprite);
}

void Sage_registerSnippet(Snippet* snippet) {
  VM_registerSnippet(sage.vm, snippet);
}

void Sage_registerEventCallback(uint32_t eventType, EventCallback_fn callback_fn, void* data) {
  EM_registerCallback(sage.em, eventType, callback_fn, data);
}
