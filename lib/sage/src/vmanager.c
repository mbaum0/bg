/**
 * @file vmanager.c
 * @author Michael Baumgarten
 * @brief ViewManager implementation
 */
#include "vmanager.h"

struct ViewManager {
    SDL_Renderer* renderer;
    SpriteArray* sprites;
    SnippetArray* snippets;
};

/**
 * @brief Used to pickup on mouse clicks on sprites
 *
 * @param data sprite array
 * @param event event that occured
 * @return int always 1 (this value is ignored)
 */
int handleEvent(void* data, SDL_Event* event) {
    SpriteArray* spriteArray = (SpriteArray*)data;
    Sprite* sprite = NULL;
    switch (event->type) {
    case SDL_MOUSEBUTTONUP:
        sprite = SpriteArray_findAtCoordinate(spriteArray, event->button.x, event->button.y);
        if (sprite != NULL) {
            if (sprite->click_fn != NULL) {
                sprite->click_fn(sprite->click_data);
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
    vm->sprites = malloc(sizeof(SpriteArray));
    SpriteArray_init(vm->sprites);
    vm->snippets = malloc(sizeof(SnippetArray));
    SnippetArray_init(vm->snippets);
    SDL_AddEventWatch(handleEvent, vm->sprites);
    return vm;
}

void VM_free(ViewManager* vm) {
    SpriteArray_free(vm->sprites);
    free(vm->sprites);
    SnippetArray_free(vm->snippets);
    free(vm->snippets);
    free(vm);
}

void VM_draw(ViewManager* vm) {
    SDL_SetRenderDrawColor(vm->renderer, 255, 255, 255, 255);
    SDL_RenderClear(vm->renderer);
    for (int32_t i = 0; i < vm->sprites->size; i++) {
        Sprite* sprite = vm->sprites->sprites[i];
        if (sprite->update_fn != NULL) {
            sprite->update_fn(sprite, sprite->update_data);
        }
        if (sprite->visible) {
            SDL_RenderCopyEx(vm->renderer, sprite->texture, &sprite->src_rect, &sprite->dst_rect, 0, NULL, sprite->flip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
        }
    }
    for (int32_t i = 0; i < vm->snippets->size; i++) {
        Snippet* snippet = vm->snippets->snippets[i];
        if (snippet->update_fn != NULL) {
            snippet->update_fn(snippet, snippet->update_data);
        }
        SDL_RenderCopy(vm->renderer, snippet->texture, NULL, &snippet->dst_rect);
    }
    SDL_RenderPresent(vm->renderer);
}

int32_t VM_registerSprite(ViewManager* vm, Sprite* sprite) {
    sprite->id = vm->sprites->size;
    SpriteArray_append(vm->sprites, sprite);
    return vm->sprites->size - 1;
}

int32_t VM_registerSnippet(ViewManager* vm, Snippet* snippet) {
    snippet->id = vm->snippets->size;
    SnippetArray_append(vm->snippets, snippet);
    return vm->snippets->size - 1;
}
