/**
 * @file vmanager.c
 * @author Michael Baumgarten
 * @brief ViewManager implementation
 */
#include "vmanager.h"
#include "stb_ds.h"

struct ViewManager {
  SDL_Renderer* renderer;
  Sprite*** sprites;
  Snippet*** snippets;
};

/**
 * @brief Used to pickup on mouse clicks on sprites
 *
 * @param data pointer to the sprite array
 * @param event event that occured
 * @return int always 1 (this value is ignored)
 */
int handleEvent(void* data, SDL_Event* event) {
  ViewManager* vm = (ViewManager*)data;
  Sprite* sprite = NULL;
  switch (event->type) {
  case SDL_MOUSEBUTTONUP:
    sprite = VM_findSpriteAtCoordinate(vm, event->button.x, event->button.y);
    if (sprite != NULL) {
      if (sprite->click_fn != NULL) {
        SpriteClick_fn fptr = (SpriteClick_fn)sprite->click_fn;
        fptr(vm, sprite, sprite->click_data);
      }
    }
    break;
  default:
    break;
  }
  return 1;
}

ViewManager* VM_init(SDL_Renderer* renderer) {
  ViewManager* vm = malloc(sizeof(ViewManager));
  vm->renderer = renderer;
  vm->sprites = malloc(sizeof(Sprite**));
  *vm->sprites = NULL;
  vm->snippets = malloc(sizeof(Snippet**));
  *vm->snippets = NULL;
  SDL_AddEventWatch(handleEvent, vm);
  return vm;
}

void VM_free(ViewManager* vm) {
  arrfree(*vm->sprites);
  free(vm->sprites);
  arrfree(*vm->snippets);
  free(vm->snippets);
  free(vm);
}

void VM_draw(ViewManager* vm) {
  SDL_SetRenderDrawColor(vm->renderer, 255, 255, 255, 255);
  SDL_RenderClear(vm->renderer);
  Sprite** sprites = *vm->sprites;
  for (int32_t i = 0; i < arrlen(sprites); i++) {
    Sprite* sprite = sprites[i];
    if (sprite->update_fn != NULL) {
      SpriteUpdate_fn fptr = (SpriteUpdate_fn)sprite->update_fn;
      fptr(vm, sprite, sprite->update_data);
    }
    if (sprite->visible) {
      SDL_RenderCopyEx(vm->renderer, sprite->texture, &sprite->src_rect, &sprite->dst_rect, 0, NULL,
                       sprite->flip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
    }
  }
  Snippet** snippets = *vm->snippets;
  for (int32_t i = 0; i < arrlen(snippets); i++) {
    Snippet* snippet = snippets[i];
    if (snippet->update_fn != NULL) {
      snippet->update_fn(snippet, snippet->update_data);
    }
    SDL_RenderCopy(vm->renderer, snippet->texture, NULL, &snippet->dst_rect);
  }
  SDL_RenderPresent(vm->renderer);
}

int32_t VM_registerSprite(ViewManager* vm, Sprite* sprite) {
  sprite->id = arrlen(*vm->sprites);
  arrput(*vm->sprites, sprite);
  return sprite->id;
}

void Sprite_registerUpdateFn(Sprite* sprite, SpriteUpdate_fn update_fn, void* data){
  sprite->update_fn = (void*)update_fn;
  sprite->update_data = data;
}
void Sprite_registerClickFn(Sprite* sprite, SpriteClick_fn click_fn, void* data){
  sprite->click_fn = (void*)click_fn;
  sprite->click_data = data;
}

int32_t VM_registerSnippet(ViewManager* vm, Snippet* snippet) {
  snippet->id = arrlen(*vm->snippets);
  arrput(*vm->snippets, snippet);
  return snippet->id;
}
Sprite* VM_findSpriteAtCoordinate(ViewManager* vm, int32_t x, int32_t y){
  Sprite** sprites = *vm->sprites;
  Sprite* topSprite = NULL;
  int32_t currentTopZ = -1;
  for (int32_t i = 0; i < arrlen(sprites); i++) {
    Sprite* sprite = sprites[i];
    if (sprite->visible) {
      SDL_Point p = {x, y};
      if (SDL_PointInRect(&p, &sprite->dst_rect)) {
        if (sprite->z > currentTopZ) {
          currentTopZ = sprite->z;
          topSprite = sprite;
        }
      }
    }
  }
  return topSprite;
}

Sprite* VM_findSpriteCollision(ViewManager* vm, Sprite* sprite){
  Sprite** sprites = *vm->sprites;
  for (int32_t i = 0; i < arrlen(sprites); i++) {
    Sprite* other = sprites[i];
    if (other != sprite && other->visible) {
      if (SDL_HasIntersection(&sprite->dst_rect, &other->dst_rect)) {
        return other;
      }
    }
  }
  return NULL;
}
