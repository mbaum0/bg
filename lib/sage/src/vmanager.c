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

Sint32 compareSpriteZ(const void* a, const void* b) {
    Sprite* aS = *(Sprite**)a;
    Sprite* bS = *(Sprite**)b;
    return aS->z - bS->z;
}

void VM_sortSprites(ViewManager* vm) {
    qsort(*vm->sprites, arrlen(*vm->sprites), sizeof(Sprite*), compareSpriteZ);
}

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
                fptr(vm, sprite, sprite->click_object, sprite->click_context, sprite->click_code);
            }
        } else {
            Snippet* snippet = VM_findSnippetAtCoordinate(vm, mouseX, mouseY);
            if (snippet != NULL) {
                if (snippet->click_fn != NULL) {
                    SnippetClick_fn fptr = (SnippetClick_fn)snippet->click_fn;
                    fptr(vm, snippet, snippet->click_object, snippet->click_context, snippet->click_code);
                }
            }
        }
        break;
    default:
        break;
    }
    return 1;
}

ViewManager* VM_init(SDL_Renderer* renderer) {
    ViewManager* vm = SDL_calloc(1, sizeof(ViewManager));
    vm->renderer = renderer;
    vm->sprites = SDL_malloc(sizeof(Sprite**));
    *vm->sprites = NULL;
    vm->snippets = SDL_malloc(sizeof(Snippet**));
    *vm->snippets = NULL;
    SDL_AddEventWatch(handleEvent, vm);
    vm->sortSpriteZNextFrame = false;
    return vm;
}

void freeSprites(ViewManager* vm) {
    Sprite** sprites = *vm->sprites;
    for (Sint32 i = 0; i < arrlen(sprites); i++) {
        free(sprites[i]);
    }
}

void freeSnippets(ViewManager* vm) {
    Snippet** snippets = *vm->snippets;
    for (Sint32 i = 0; i < arrlen(snippets); i++) {
        free(snippets[i]->text);
        free(snippets[i]);
    }
}

void VM_free(ViewManager* vm) {
    freeSprites(vm);
    arrfree(*vm->sprites);
    free(vm->sprites);
    freeSnippets(vm);
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
    if (vm->sortSpriteZNextFrame) {
        VM_sortSprites(vm);
        vm->sortSpriteZNextFrame = false;
    }

    for (Sint32 i = 0; i < arrlen(sprites); i++) {
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
        }
    }
    Snippet** snippets = *vm->snippets;
    for (Sint32 i = 0; i < arrlen(snippets); i++) {
        Snippet* snippet = snippets[i];
        if (snippet->update_fn != NULL) {
            SnippetUpdate_fn fptr = (SnippetUpdate_fn)snippet->update_fn;
            fptr(vm, snippet, snippet->update_data);
        }

        Sint32 alignOffset = 0;
        if (snippet->centerAlign) {
            alignOffset -= (snippet->pxLen / 2);
        }

        if (snippet->visible) {
            for (Sint32 i = 0; i < snippet->textLen; i++) {
                SnippetChar sc = snippet->chars[i];
                SDL_FRect newDst = sc.dst;
                if (snippet->useViewport) {
                    newDst.x += vm->viewport.x;
                    newDst.y += vm->viewport.y;
                }

                newDst.x += alignOffset;

                SDL_SetTextureColorMod(snippet->font->texture, snippet->color.r, snippet->color.g, snippet->color.b);
                SDL_RenderTexture(vm->renderer, snippet->font->texture, &sc.src, &newDst);
            }
        }
    }
    SDL_RenderPresent(vm->renderer);
}

Sint32 VM_registerSprite(ViewManager* vm, Sprite* sprite) {
    sprite->id = (Sint32)arrlen(*vm->sprites);
    Sint32 zindex = -1;
    for (Sint32 i = 0; i < arrlen(*vm->sprites); i++) {
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

void VM_setSpriteZ(ViewManager* vm, Sprite* s, Sint32 newZ) {
    if (s->z != newZ) {
        s->z = newZ;
        vm->sortSpriteZNextFrame = true;
    }
}

void Sprite_registerUpdateFn(Sprite* sprite, SpriteUpdate_fn update_fn, void* object, void* context) {
    sprite->update_fn = (void*)update_fn;
    sprite->update_object = object;
    sprite->update_context = context;
}

void Sprite_registerClickFn(Sprite* sprite, SpriteClick_fn click_fn, void* object, void* context, Sint32 code) {
    sprite->click_fn = (void*)click_fn;
    sprite->click_object = object;
    sprite->click_context = context;
    sprite->click_code = code;
}

Sint32 VM_registerSnippet(ViewManager* vm, Snippet* snippet) {
    snippet->id = (Sint32)arrlen(*vm->snippets);
    arrput(*vm->snippets, snippet);
    return snippet->id;
}

void Snippet_registerClickFn(Snippet* snippet, SnippetClick_fn click_fn, void* object, void* context, Sint32 code) {
    snippet->click_fn = (void*)click_fn;
    snippet->click_object = object;
    snippet->click_context = context;
    snippet->click_code = code;
}

void Snippet_registerUpdateFn(Snippet* snippet, SnippetUpdate_fn update_fn, void* data) {
    snippet->update_fn = (void*)update_fn;
    snippet->update_data = data;
}

/**
 * @brief Returns the first snippet found at the given location
 */
Snippet* VM_findSnippetAtCoordinate(ViewManager* vm, Sint32 x, Sint32 y) {
    Snippet** snippets = *vm->snippets;
    for (Sint32 i = 0; i < arrlen(snippets); i++) {
        Snippet* snippet = snippets[i];
        if (snippet->click_fn != NULL) {
            SDL_FPoint p = {x, y};
            SDL_FRect dst = snippet->boundBox;
            if (snippet->useViewport) {
                dst.x += vm->viewport.x;
                dst.y += vm->viewport.y;
            }
            if (SDL_PointInRectFloat(&p, &dst)) {
                return snippet;
            }
        }
    }
    return NULL;
}

Sprite* VM_findSpriteAtCoordinate(ViewManager* vm, Sint32 x, Sint32 y) {
    Sprite** sprites = *vm->sprites;
    Sprite* topSprite = NULL;
    Sint32 currentTopZ = -1;
    for (Sint32 i = 0; i < arrlen(sprites); i++) {
        Sprite* sprite = sprites[i];
        if (sprite->visible && sprite->click_fn != NULL) {
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
    for (Sint32 i = 0; i < arrlen(sprites); i++) {
        Sprite* other = sprites[i];
        if (other != sprite && other->visible) {
            if (SDL_HasRectIntersectionFloat(&sprite->dst_rect, &other->dst_rect)) {
                return other;
            }
        }
    }
    return NULL;
}
