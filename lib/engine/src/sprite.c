/**
 * @file sprite.c
 * @author Michael Baumgarten
 * @brief Sprite implementation
 */
#include "sprite.h"


void SpriteArray_init(SpriteArray* spriteArray) {
    spriteArray->sprites = NULL;
    spriteArray->size = 0;
}

void SpriteArray_append(SpriteArray* spriteArray, Sprite* sprite) {
    spriteArray->size++;
    spriteArray->sprites = realloc(spriteArray->sprites, spriteArray->size * sizeof(Sprite*));
    spriteArray->sprites[spriteArray->size - 1] = sprite;
}

void SpriteArray_free(SpriteArray* spriteArray) {
    for (int32_t i = 0; i < spriteArray->size; i++) {
        free(spriteArray->sprites[i]);
    }
    free(spriteArray->sprites);
    spriteArray->size = 0;
}

Sprite* SpriteArray_findAtCoordinate(SpriteArray* spriteArray, int32_t x, int32_t y){
    Sprite* topSprite = NULL;
    int32_t currentTopZ = -1;
    for (int32_t i = 0; i < spriteArray->size; i++) {
        Sprite* sprite = spriteArray->sprites[i];
        if (sprite->visible) {
            SDL_Point p = { x, y };
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

void Sprite_setLocation(Sprite* sprite, int32_t x, int32_t y) {
    sprite->dst_rect.x = x;
    sprite->dst_rect.y = y;
}

int32_t Sprite_getX(Sprite* sprite) {
    return sprite->dst_rect.x;
}

int32_t Sprite_getY(Sprite* sprite) {
    return sprite->dst_rect.y;
}

void Sprite_setVisible(Sprite* sprite, bool visible) {
    sprite->visible = visible;
}

void Sprite_setSourceRect(Sprite* sprite, SDL_Rect src) {
    sprite->src_rect = src;
}

void Sprite_setFrame(Sprite* sprite, uint32_t frame){
    sprite->frame = frame;
}

uint32_t Sprite_getFrame(Sprite* sprite){
    return sprite->frame;
}
