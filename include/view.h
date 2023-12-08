/**
 * @file view.h
 * @author Michael Baumgarten
 * @brief ViewManager is responsible for:
 *  1. Managing sprites
 *  2. Rendering sprites to the screen
 *  3. Passing user input events to the controller
 */
#pragma once
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "events.h"

#define BOARD_HEIGHT 1080
#define BOARD_WIDTH 1560

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
 * @brief ViewManager manages rendering Sprites to the screen.
 */
typedef struct ViewManager ViewManager;

/**
 * @brief Initialize a new ViewManager instance and returns a pointer to it. Must
 * be freed with VM_free.
 */
ViewManager* VM_init(SDL_Renderer* renderer);

/**
 * @brief Free memory allocated by the ViewManager, including Sprites
 */
void VM_free(ViewManager* vm);

/**
 * @brief Render the sprites to the screen
 */
void VM_draw(ViewManager* vm);


/**
 * @brief Create a new managed Sprite instance. Do not free Sprites directly, as they are managed by the ViewManager.
 *
 * @param vm The ViewManager instance
 * @param texture The SDL_Texture to use for this Sprite
 * @param src The source rectangle to use for this Sprite
 * @param x The x coordinate of the Sprite
 * @param y The y coordinate of the Sprite
 * @param update_fn The update function to call every frame
 * @param update_data The data to pass to the update function
 *
 * @return The ID of the new Sprite
 */
uint32_t VM_createSprite(ViewManager* vm, SDL_Texture* texture, SDL_Rect src, uint32_t x, uint32_t y, SpriteUpdate_fn update_fn, void* update_data);

/**
 * @brief Set the location of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setLocation(Sprite* sprite, uint32_t x, uint32_t y);

/**
 * @brief Get the x coordinate of the sprite
 */
uint32_t Sprite_getX(Sprite* sprite);

/**
 * @brief Get the y coordinate of the sprite
 */
uint32_t Sprite_getY(Sprite* sprite);

/**
 * @brief Set the visibility of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setVisible(Sprite* sprite, bool visible);

/**
 * @brief Set the source rectangle of the sprite. Only used in SpriteUpdate_fn callbacks
 */
void Sprite_setSourceRect(Sprite* sprite, SDL_Rect src);
