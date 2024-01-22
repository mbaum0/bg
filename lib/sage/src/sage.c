/**
 * @file sage.c
 * @author Michael Baumgarten
 * @brief Simply-Another-Game-Engine (SAGE)
 */
#include <stdlib.h>
#include "sage.h"

int32_t delayFrame(int32_t frameStart) {
  int32_t frameTime = SDL_GetTicks() - frameStart;
  if (frameTime < MS_PER_FRAME) {
    SDL_Delay(MS_PER_FRAME - frameTime);
  }
  return SDL_GetTicks();
}

Sage* Sage_create(char* title, int width, int height) {
  Sage* s = malloc(sizeof(Sage));
  s->mm = MM_init(title, width, height);
  s->vm = VM_init(s->mm->renderer);
  s->em = EM_init();
  s->width = width;
  s->height = height;
  return s;
}

void Sage_setNormalRect(Sage* s, SDL_FRect rect){
  VM_setNormalRect(s->vm, rect);
}

void Sage_destroy(Sage* s) {
  VM_free(s->vm);
  MM_free(s->mm);
  free(s);
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

SDL_Texture* Sage_loadTexture(Sage* s, char* path) {
  return MM_loadTexture(s->mm, path);
}

TTF_Font* Sage_loadFont(Sage* s, char* path, int32_t size) {
  return MM_loadFont(s->mm, path, size);
}

int32_t Sage_registerSprite(Sage* s, Sprite* sprite) {
  return VM_registerSprite(s->vm, sprite);
}

void Sage_registerSnippet(Sage* s, Snippet* snippet) {
  VM_registerSnippet(s->vm, snippet);
}

void Sage_registerEventCallback(Sage* s, uint32_t eventType, EventCallback_fn callback_fn, void* data) {
  EM_registerCallback(s->em, eventType, callback_fn, data);
}
