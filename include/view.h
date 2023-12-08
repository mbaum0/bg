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
#define WINDOW_SIDE_OFFSET ((WINDOW_WIDTH - BOARD_WIDTH) / 2)
#define WINDOW_TOP_OFFSET ((WINDOW_HEIGHT - BOARD_HEIGHT) / 2)
#define PLAY_AREA_TOP_OFFSET (WINDOW_TOP_OFFSET + 40)
#define CENTER_GAP_WIDTH 90
#define PIP_WIDTH 90
#define PIP_Y_GAP 100
#define PIP_HEIGHT (PIP_WIDTH * 5)
#define CHECKER_SIZE 80
#define PIP_CHECKER_OFFSET ((PIP_WIDTH - CHECKER_SIZE) / 2)
#define PLAY_AREA_LEFT_SIDE_X_OFFSET (WINDOW_SIDE_OFFSET + 195)
#define PLAY_AREA_RIGHT_SIDE_X_OFFSET (PLAY_AREA_LEFT_SIDE_X_OFFSET + CENTER_GAP_WIDTH + (6 * PIP_WIDTH))
#define GET_CHECKER_X(pip) \
    ((pip < 1 || pip > 24) ? 0 : \
        ((pip >= 1 && pip <= 6) ? (PLAY_AREA_RIGHT_SIDE_X_OFFSET + PIP_CHECKER_OFFSET + ((6 - pip) * PIP_WIDTH)) : \
        ((pip >= 7 && pip <= 12) ? (PLAY_AREA_LEFT_SIDE_X_OFFSET + PIP_CHECKER_OFFSET + ((12 - pip) * PIP_WIDTH)) : \
        ((pip >= 13 && pip <= 18) ? (PLAY_AREA_LEFT_SIDE_X_OFFSET + PIP_CHECKER_OFFSET + ((pip - 13) * PIP_WIDTH)) : \
        (PLAY_AREA_RIGHT_SIDE_X_OFFSET + PIP_CHECKER_OFFSET + ((pip - 19) * PIP_WIDTH))))))
#define GET_CHECKER_Y(pip, index) \
    ((pip < 1 || pip > 24) ? 0 : \
    ((pip >= 1 && pip <= 12) ? (PLAY_AREA_TOP_OFFSET + BOARD_HEIGHT - ((2 +  index) * CHECKER_SIZE)) : \
    (PLAY_AREA_TOP_OFFSET + ((index) * CHECKER_SIZE))))

#define Z_BOARD 0
#define Z_CHECKER 1

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
uint32_t VM_createSprite(ViewManager* vm, SDL_Texture* texture, SDL_Rect src, uint32_t x, uint32_t y, uint32_t z, SpriteUpdate_fn update_fn, void* update_data);

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
