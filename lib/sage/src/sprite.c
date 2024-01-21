/**
 * @file sprite.c
 * @author Michael Baumgarten
 * @brief Sprite implementation
 */
#include "sprite.h"
#include "stb_ds.h"

Sprite* Sprite_create(SDL_Texture* texture, SDL_Rect src, int32_t x, int32_t y, int32_t z) {
  Sprite* sprite = malloc(sizeof(Sprite));
  sprite->id = 0;
  sprite->texture = texture;
  sprite->src_rect = src;
  sprite->dstn_rect = (SDL_FRect){x, y, src.w, src.h};
  sprite->visible = true;
  sprite->hovered = false;
  sprite->update_fn = NULL;
  sprite->update_object = NULL;
  sprite->update_context = NULL;
  sprite->click_fn = NULL;
  sprite->click_object = NULL;
  sprite->click_context = NULL;
  sprite->z = z;
  sprite->flip = false;
  sprite->normalized = false;
  sprite->lockAspectRatio = false;
  sprite->yRelativeToBottom = false;
  sprite->frame = 0;
  return sprite;
}

Sprite* Sprite_createEx(SDL_Texture* texture, SDL_Rect src, SDL_FRect normDst, int32_t z, bool flip, bool normalized, bool lockAspectRatio, bool yRelativeToBottom){
  Sprite* sprite = malloc(sizeof(Sprite));
  sprite->id = 0;
  sprite->texture = texture;
  sprite->src_rect = src;
  sprite->dstn_rect = normDst;
  sprite->visible = true;
  sprite->hovered = false;
  sprite->update_fn = NULL;
  sprite->update_object = NULL;
  sprite->update_context = NULL;
  sprite->click_fn = NULL;
  sprite->click_object = NULL;
  sprite->click_context = NULL;
  sprite->z = z;
  sprite->flip = flip;
  sprite->normalized = normalized;
  sprite->lockAspectRatio = lockAspectRatio;
  sprite->yRelativeToBottom = yRelativeToBottom;
  sprite->frame = 0;
  return sprite;
}

void Sprite_setLocation(Sprite* sprite, float x, float y) {
  sprite->dstn_rect.x = x;
  sprite->dstn_rect.y = y;
}

float Sprite_getX(Sprite* sprite) {
  return sprite->dstn_rect.x;
}

float Sprite_getY(Sprite* sprite) {
  return sprite->dstn_rect.y;
}

void Sprite_setVisible(Sprite* sprite, bool visible) {
  sprite->visible = visible;
}

void Sprite_setSourceRect(Sprite* sprite, SDL_Rect src) {
  sprite->src_rect = src;
}

void Sprite_setFrame(Sprite* sprite, uint32_t frame) {
  sprite->frame = frame;
}

uint32_t Sprite_getFrame(Sprite* sprite) {
  return sprite->frame;
}
