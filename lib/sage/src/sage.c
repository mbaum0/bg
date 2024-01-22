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

void Sage_init(char* title, int width, int height) {
  sage.mm = MM_init(title, width, height);
  sage.vm = VM_init(sage.mm->renderer);
  sage.em = EM_init();
  sage.width = width;
  sage.height = height;
}

void Sage_setNormalRect(SDL_FRect rect){
  VM_setNormalRect(sage.vm, rect);
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

void Sage_run(Sage* s) {
  while (processInput() == false) {
    int32_t frameStart = SDL_GetTicks();
    VM_draw(s->vm);
    int32_t frameEnd = delayFrame(frameStart);
    (void)frameEnd;
    //stats.fps = 1000.0 / (frameEnd - frameStart);
  }
}

SDL_Texture* Sage_loadTexture(char* path) {
  return MM_loadTexture(sage.mm, path);
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
