/**
 * @file view.c
 * @author Michael Baumgarten
 * @brief ViewManager implementation
 */
#include "view.h"

typedef struct SpriteArray SpriteArray;

struct ViewManager {
    SDL_Renderer* renderer;
    SpriteArray* sprites;
};

struct SpriteArray {
    Sprite** sprites;
    uint32_t size;
};

struct Sprite {
    SDL_Texture* texture;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    bool visible;
    SpriteUpdate_fn update_fn;
    void* update_data;
};

void initSpriteArray(SpriteArray* spriteArray) {
    spriteArray->sprites = NULL;
    spriteArray->size = 0;
}

void appendSprite(SpriteArray* spriteArray, Sprite* sprite) {
    spriteArray->size++;
    spriteArray->sprites = realloc(spriteArray->sprites, spriteArray->size * sizeof(Sprite*));
    spriteArray->sprites[spriteArray->size - 1] = sprite;
}

void freeSpriteArray(SpriteArray* spriteArray) {
    for (size_t i = 0; i < spriteArray->size; i++) {
        free(spriteArray->sprites[i]);
    }
    free(spriteArray->sprites);
    spriteArray->size = 0;
}

ViewManager* VM_init(SDL_Renderer* renderer) {
    ViewManager* vm = malloc(sizeof(ViewManager));
    vm->renderer = renderer;
    vm->sprites = malloc(sizeof(SpriteArray));
    initSpriteArray(vm->sprites);
    return vm;
}

void VM_free(ViewManager* vm) {
    freeSpriteArray(vm->sprites);
    free(vm->sprites);
    free(vm);
}

void VM_draw(ViewManager* vm) {
    SDL_RenderClear(vm->renderer);
    for (size_t i = 0; i < vm->sprites->size; i++) {
        Sprite* sprite = vm->sprites->sprites[i];
        if (sprite->update_fn != NULL) {
            sprite->update_fn(sprite, sprite->update_data);
        }
        if (sprite->visible) {
            SDL_RenderCopy(vm->renderer, sprite->texture, &sprite->src_rect, &sprite->dst_rect);
        }
    }
    SDL_RenderPresent(vm->renderer);
}

uint32_t VM_createSprite(ViewManager* vm, SDL_Texture* texture, SDL_Rect src, uint32_t x, uint32_t y, SpriteUpdate_fn update_fn, void* update_data){
    Sprite* sprite = malloc(sizeof(Sprite));
    sprite->texture = texture;
    sprite->src_rect = src;
    sprite->dst_rect = (SDL_Rect){x, y, src.w, src.h};
    sprite->visible = true;
    sprite->update_fn = update_fn;
    sprite->update_data = update_data;
    appendSprite(vm->sprites, sprite);
    return vm->sprites->size - 1;
}

void Sprite_setLocation(Sprite* sprite, uint32_t x, uint32_t y) {
    sprite->dst_rect.x = x;
    sprite->dst_rect.y = y;
}

uint32_t Sprite_getX(Sprite* sprite){
    return sprite->dst_rect.x;
}

uint32_t Sprite_getY(Sprite* sprite){
    return sprite->dst_rect.y;
}

void Sprite_setVisible(Sprite* sprite, bool visible) {
    sprite->visible = visible;
}

void Sprite_setSourceRect(Sprite* sprite, SDL_Rect src) {
    sprite->src_rect = src;
}
