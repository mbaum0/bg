/**
 * @file sprite.c
 * @author Michael Baumgarten
 * @brief Sprite implementation
 */
#include "sprite.h"
#include "stb_ds.h"

Sprite* Sprite_findAtCoordinate(Sprite** sprites, int32_t x, int32_t y) {
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

Sprite* Sprite_create(SDL_Texture* texture, SDL_Rect src, int32_t x, int32_t y, int32_t z, bool flip,
                      SpriteUpdate_fn update_fn, void* update_data, SpriteClick_fn click_fn, void* click_data) {
  Sprite* sprite = malloc(sizeof(Sprite));
  sprite->id = 0;
  sprite->texture = texture;
  sprite->src_rect = src;
  sprite->dst_rect = (SDL_Rect){x, y, src.w, src.h};
  sprite->visible = true;
  sprite->hovered = false;
  sprite->update_fn = update_fn;
  sprite->update_data = update_data;
  sprite->click_fn = click_fn;
  sprite->click_data = click_data;
  sprite->z = z;
  sprite->flip = flip;
  sprite->frame = 0;
  return sprite;
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

void Sprite_setFrame(Sprite* sprite, uint32_t frame) {
  sprite->frame = frame;
}

uint32_t Sprite_getFrame(Sprite* sprite) {
  return sprite->frame;
}
