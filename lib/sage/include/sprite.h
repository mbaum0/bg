/**
 * @file sprite.h
 * @author Michael Baumgarten
 * @brief Sprite implementation for the game engine
 */
#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief A Sprite is a 2D image that is drawn on the screen. It can be manipulated using
 * the various Sprite_* functions.
 */
typedef struct Sprite Sprite;

struct Sprite {
  int32_t id;
  SDL_Texture* texture;
  SDL_FRect src_rect;
  SDL_FRect dst_rect;
  bool visible;
  bool hovered;
  void* update_fn;
  void* update_object;
  void* update_context;
  void* click_fn;
  void* click_object;
  void* click_context;
  bool useViewport;
  int32_t z;
};

/**
 * @brief Create a new Sprite instance
 *
 * @param texture The SDL_Texture to use for this sprite
 * @param src The source rectangle
 * @param x The x coordinate
 * @param y The y coordinate
 * @param z The z coordinate
 * @return Sprite* The new Sprite instance
 */
Sprite* Sprite_create(SDL_Texture* texture, SDL_FRect src, int32_t x, int32_t y, int32_t z);


Sprite* Sprite_createEx(SDL_Texture* texture, SDL_FRect src, SDL_FRect dst, int32_t z);

/**
 * @brief Set the location of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setLocation(Sprite* sprite, float x, float y);

/**
 * @brief Get the x coordinate of the sprite
 */
float Sprite_getX(Sprite* sprite);

/**
 * @brief Get the y coordinate of the sprite
 */
float Sprite_getY(Sprite* sprite);

/**
 * @brief Set the visibility of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setVisible(Sprite* sprite, bool visible);

/**
 * @brief Set the source rectangle of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setSourceRect(Sprite* sprite, SDL_FRect src);

/**
 * @brief Set the frame of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setFrame(Sprite* sprite, uint32_t frame);

/**
 * @brief Get the frame of the sprite
 */
uint32_t Sprite_getFrame(Sprite* sprite);
