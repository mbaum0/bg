/**
 * @file sprite.h
 * @author Michael Baumgarten
 * @brief Sprite implementation for the game engine
 */
#pragma once
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "vmanager.h"

 /**
  * @brief A Sprite is a 2D image that is drawn on the screen. It can be manipulated using
  * the various Sprite_* functions.
  */
typedef struct Sprite Sprite;

/**
 * @brief Sprite update functions are called every frame to update the Sprite's state.
 */
typedef void (*SpriteUpdate_fn)(Sprite* sprite, void* data);

/**
 * @brief Sprite click functions are called when the Sprite is clicked.
 */
typedef void (*SpriteClick_fn)(void* data);

/**
 * @brief Create a new managed Sprite instance. Do not free Sprites directly, as they are managed by the ViewManager.
 *
 * @param vm The ViewManager instance
 * @param texture The SDL_Texture to use for this Sprite
 * @param src The source rectangle to use for this Sprite
 * @param x The x coordinate of the Sprite
 * @param y The y coordinate of the Sprite
 * @param z The z coordinate of the Sprite
 * @param flip Flip the Texture for the Sprite
 * @param update_fn The update function to call every frame
 * @param update_data The data to pass to the update function
 * @param click_fn The click function to call when the Sprite is clicked
 * @param click_data The data to pass to the click function
 *
 * @return The ID of the new Sprite
 */
int32_t VM_createSprite(ViewManager* vm, SDL_Texture* texture, SDL_Rect src, int32_t x, int32_t y, int32_t z, bool flip, SpriteUpdate_fn update_fn, void* update_data, SpriteClick_fn click_fn, void* click_data);

/**
 * @brief Set the location of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setLocation(Sprite* sprite, int32_t x, int32_t y);

/**
 * @brief Get the x coordinate of the sprite
 */
int32_t Sprite_getX(Sprite* sprite);

/**
 * @brief Get the y coordinate of the sprite
 */
int32_t Sprite_getY(Sprite* sprite);

/**
 * @brief Set the visibility of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setVisible(Sprite* sprite, bool visible);

/**
 * @brief Set the source rectangle of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setSourceRect(Sprite* sprite, SDL_Rect src);

/**
 * @brief Set the frame of the sprite. Only used in SpriteUpdate_fn callbacks
*/
void Sprite_setFrame(Sprite* sprite, uint32_t frame);

/**
 * @brief Get the frame of the sprite
*/
uint32_t Sprite_getFrame(Sprite* sprite);
