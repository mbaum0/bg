/**
 * @file vmanager.c
 * @author Michael Baumgarten
 * @brief ViewManager implementation
 */
#include <math.h>
#include "vmanager.h"
#include "stb_ds.h"
#include "log.h"
#include "util.h"
#include "log.h"

struct ViewManager {
  SDL_Renderer* renderer;
  Sprite*** sprites;
  Snippet*** snippets;
  SDL_Rect normalRect;
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
  float mouseX, mouseY;
  switch (event->type) {
  case SDL_MOUSEBUTTONUP:
    mouseX = (event->button.x / 1920.0);
    mouseY = (event->button.y / 1080.0);
    log_debug("mouse up at %f, %f", mouseX, mouseY);
    sprite = VM_findSpriteAtCoordinate(vm, mouseX, mouseY);
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
  vm->normalRect = (SDL_Rect){ 0, 0, 0, 0 };
  return vm;
}

void VM_setNormalRect(ViewManager* vm, SDL_Rect rect) {
  vm->normalRect = rect;
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
      SDL_Rect normalDst = (SDL_Rect){ sprite->dstn_rect.x, sprite->dstn_rect.y, sprite->dstn_rect.w, sprite->dstn_rect.h };
      if (sprite->normalized) {
        normalDst.x = (sprite->dstn_rect.x * vm->normalRect.w) + vm->normalRect.x;
        normalDst.y = (sprite->dstn_rect.y * vm->normalRect.h) + vm->normalRect.y;
        normalDst.w = sprite->dstn_rect.w * vm->normalRect.w;
        normalDst.h = sprite->dstn_rect.h * vm->normalRect.h;

        float normalHeight = sprite->dstn_rect.h;
        if (sprite->lockAspectRatio) {
          float aspectRatio = (float)sprite->src_rect.w / (float)sprite->src_rect.h;
          normalHeight = sprite->dstn_rect.w  / aspectRatio;
          normalDst.h = normalHeight * vm->normalRect.w;
        }
        if (sprite->yRelativeToBottom) {
          float normalY = 1.0 - sprite->dstn_rect.y;
          normalDst.y = (normalY * vm->normalRect.h) + vm->normalRect.y - normalDst.h;
        }
      }
      SDL_RenderCopyEx(vm->renderer, sprite->texture, &sprite->src_rect, &normalDst, 0, NULL,
        sprite->flip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
    }
  }
  Snippet** snippets = *vm->snippets;
  for (int32_t i = 0; i < arrlen(snippets); i++) {
    Snippet* snippet = snippets[i];
    if (snippet->update_fn != NULL) {
      SnippetUpdate_fn fptr = (SnippetUpdate_fn)snippet->update_fn;
      fptr(vm, snippet, snippet->update_data);
    }
    SDL_RenderCopy(vm->renderer, snippet->texture, NULL, &snippet->dst_rect);
  }
  SDL_RenderPresent(vm->renderer);
}

int32_t VM_registerSprite(ViewManager* vm, Sprite* sprite) {
  sprite->id = arrlen(*vm->sprites);
  int32_t zindex = -1;
  for (int32_t i = 0; i < arrlen(*vm->sprites); i++) {
    Sprite* s = (*vm->sprites)[i];
    if (s->z > sprite->z) {
      zindex = i;
      break;
    }
  }
  if (zindex == -1) {
    arrput(*vm->sprites, sprite);
  }
  else {
    arrins(*vm->sprites, zindex, sprite);
  }
  return sprite->id;
}

void Sprite_registerUpdateFn(Sprite* sprite, SpriteUpdate_fn update_fn, void* data) {
  sprite->update_fn = (void*)update_fn;
  sprite->update_data = data;
}

void Sprite_registerClickFn(Sprite* sprite, SpriteClick_fn click_fn, void* data) {
  sprite->click_fn = (void*)click_fn;
  sprite->click_data = data;
}

int32_t VM_registerSnippet(ViewManager* vm, Snippet* snippet) {
  snippet->renderer = vm->renderer;
  snippet->id = arrlen(*vm->snippets);
  arrput(*vm->snippets, snippet);
  return snippet->id;
}

void Snippet_registerUpdateFn(Snippet* snippet, SnippetUpdate_fn update_fn, void* data) {
  snippet->update_fn = (void*)update_fn;
  snippet->update_data = data;
}

Sprite* VM_findSpriteAtCoordinate(ViewManager* vm, float x, float y) {
  Sprite** sprites = *vm->sprites;
  Sprite* topSprite = NULL;
  int32_t currentTopZ = -1;
  for (int32_t i = 0; i < arrlen(sprites); i++) {
    Sprite* sprite = sprites[i];
    if (sprite->visible) {
      SDL_FPoint p = { x, y };
      if (SDL_PointInFRect(&p, &sprite->dstn_rect)) {
        if (sprite->z > currentTopZ) {
          currentTopZ = sprite->z;
          topSprite = sprite;
        }
      }
    }
  }
  return topSprite;
}

Sprite* VM_findSpriteCollision(ViewManager* vm, Sprite* sprite) {
  Sprite** sprites = *vm->sprites;
  for (int32_t i = 0; i < arrlen(sprites); i++) {
    Sprite* other = sprites[i];
    if (other != sprite && other->visible) {
      if (SDL_HasIntersectionF(&sprite->dstn_rect, &other->dstn_rect)) {
        return other;
      }
    }
  }
  return NULL;
}
