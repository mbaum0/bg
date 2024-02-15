/**
 * @file sprite.c
 * @author Michael Baumgarten
 * @brief Sprite implementation
 */
#include "sprite.h"
#include "stb_ds.h"

Sprite* Sprite_create(SDL_Texture* texture, SDL_FRect src, int32_t x, int32_t y, int32_t z) {
    Sprite* sprite = calloc(1, sizeof(Sprite));
    sprite->id = 0;
    sprite->texture = texture;
    sprite->src_rect = src;
    sprite->dst_rect = (SDL_FRect){x, y, src.w, src.h};
    sprite->visible = true;
    sprite->hovered = false;
    sprite->update_fn = NULL;
    sprite->update_object = NULL;
    sprite->update_context = NULL;
    sprite->click_fn = NULL;
    sprite->click_object = NULL;
    sprite->click_context = NULL;
    sprite->z = z;
    sprite->alpha = 255;
    sprite->useViewport = false;
    sprite->outline = (SDL_Color){0, 0, 0, 0};
    return sprite;
}

Sprite* Sprite_createEx(SDL_Texture* texture, SDL_FRect src, SDL_FRect dst, int32_t z) {
    Sprite* sprite = calloc(1, sizeof(Sprite));
    sprite->id = 0;
    sprite->texture = texture;
    sprite->src_rect = src;
    sprite->dst_rect = dst;
    sprite->visible = true;
    sprite->hovered = false;
    sprite->update_fn = NULL;
    sprite->update_object = NULL;
    sprite->update_context = NULL;
    sprite->click_fn = NULL;
    sprite->click_object = NULL;
    sprite->click_context = NULL;
    sprite->z = z;
    sprite->alpha = 255;
    sprite->useViewport = false;
    sprite->outline = (SDL_Color){0, 0, 0, 0};
    return sprite;
}

void Sprite_setLocation(Sprite* sprite, float x, float y) {
    sprite->dst_rect.x = x;
    sprite->dst_rect.y = y;
}

float Sprite_getX(Sprite* sprite) {
    return sprite->dst_rect.x;
}

float Sprite_getY(Sprite* sprite) {
    return sprite->dst_rect.y;
}

void Sprite_setVisible(Sprite* sprite, bool visible) {
    sprite->visible = visible;
}

void Sprite_setSourceRect(Sprite* sprite, SDL_FRect src) {
    sprite->src_rect = src;
}

void Sprite_setAlpha(Sprite* sprite, uint8_t alpha) {
    sprite->alpha = alpha;
}

void Sprite_setOutline(Sprite* sprite, SDL_Color color) {
    sprite->outline = color;
}
