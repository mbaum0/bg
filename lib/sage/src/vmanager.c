/**
 * @file vmanager.c
 * @author Michael Baumgarten
 * @brief ViewManager implementation
 */
#include "vmanager.h"
#include "log.h"
#include "stb_ds.h"
#include "util.h"
#include <math.h>

struct ViewManager {
  SDL_Renderer* renderer;
  Sprite*** sprites;
  Snippet*** snippets;
  SDL_FRect viewport;
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
  SDL_Event cpy = *event;
  SDL_ConvertEventToRenderCoordinates(vm->renderer, &cpy);
  Sprite* sprite = NULL;
  float mouseX, mouseY;
  switch (cpy.type) {
  case SDL_EVENT_MOUSE_BUTTON_UP:
    mouseX = cpy.button.x;
    mouseY = cpy.button.y;
    sprite = VM_findSpriteAtCoordinate(vm, mouseX, mouseY);
    if (sprite != NULL) {
      if (sprite->click_fn != NULL) {
        SpriteClick_fn fptr = (SpriteClick_fn)sprite->click_fn;
        fptr(vm, sprite, sprite->click_object, sprite->click_context);
      }
    }
    break;
  default:
    break;
  }
  return 1;
}

ViewManager* VM_init(SDL_Renderer* renderer) {
  ViewManager* vm = calloc(1, sizeof(ViewManager));
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

void VM_setViewport(ViewManager* vm, SDL_FRect viewport) {
  vm->viewport = viewport;
}

void drawSpriteBorder(ViewManager* vm, SDL_FRect border, SDL_Color color) {
  SDL_SetRenderDrawBlendMode(vm->renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(vm->renderer, color.r, color.g, color.b, color.a);
  SDL_RenderRect(vm->renderer, &border);
}

void VM_draw(ViewManager* vm) {
  SDL_SetRenderDrawColor(vm->renderer, 255, 255, 255, 255);
  SDL_RenderClear(vm->renderer);
  Sprite** sprites = *vm->sprites;
  for (int32_t i = 0; i < arrlen(sprites); i++) {
    Sprite* sprite = sprites[i];
    if (sprite->update_fn != NULL) {
      SpriteUpdate_fn fptr = (SpriteUpdate_fn)sprite->update_fn;
      fptr(vm, sprite, sprite->update_object, sprite->update_context);
    }
    if (sprite->visible) {
      SDL_FRect newDst = sprite->dst_rect;
      if (sprite->useViewport) {
        newDst.x += vm->viewport.x;
        newDst.y += vm->viewport.y;
      }
      SDL_SetTextureAlphaMod(sprite->texture, sprite->alpha);
      SDL_RenderTexture(vm->renderer, sprite->texture, &sprite->src_rect, &newDst);
      drawSpriteBorder(vm, newDst, sprite->outline);
    }
  }
  Snippet** snippets = *vm->snippets;
  for (int32_t i = 0; i < arrlen(snippets); i++) {
    Snippet* snippet = snippets[i];
    if (snippet->update_fn != NULL) {
      SnippetUpdate_fn fptr = (SnippetUpdate_fn)snippet->update_fn;
      fptr(vm, snippet, snippet->update_data);
    }
    SDL_RenderTexture(vm->renderer, snippet->texture, NULL, &snippet->dst_rect);
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
  } else {
    arrins(*vm->sprites, zindex, sprite);
  }
  return sprite->id;
}

void Sprite_registerUpdateFn(Sprite* sprite, SpriteUpdate_fn update_fn, void* object, void* context) {
  sprite->update_fn = (void*)update_fn;
  sprite->update_object = object;
  sprite->update_context = context;
}

void Sprite_registerClickFn(Sprite* sprite, SpriteClick_fn click_fn, void* object, void* context) {
  sprite->click_fn = (void*)click_fn;
  sprite->click_object = object;
  sprite->click_context = context;
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

Sprite* VM_findSpriteAtCoordinate(ViewManager* vm, int32_t x, int32_t y) {
  Sprite** sprites = *vm->sprites;
  Sprite* topSprite = NULL;
  int32_t currentTopZ = -1;
  for (int32_t i = 0; i < arrlen(sprites); i++) {
    Sprite* sprite = sprites[i];
    if (sprite->visible) {
      SDL_FPoint p = {x, y};
      SDL_FRect dst = sprite->dst_rect;
      if (sprite->useViewport) {
        dst.x += vm->viewport.x;
        dst.y += vm->viewport.y;
      }
      if (SDL_PointInRectFloat(&p, &dst)) {
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
      if (SDL_HasRectIntersectionFloat(&sprite->dst_rect, &other->dst_rect)) {
        return other;
      }
    }
  }
  return NULL;
}
