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
#include <SDL_ttf.h>

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

#define GET_PIP_X(pip) \
    ((pip >= 0 && pip <= 5) ? (PLAY_AREA_RIGHT_SIDE_X_OFFSET + ((5 - pip) * PIP_WIDTH)) : \
    ((pip >= 6 && pip <= 11) ? (PLAY_AREA_LEFT_SIDE_X_OFFSET + ((11 - pip) * PIP_WIDTH)) : \
    ((pip >= 12 && pip <= 17) ? (PLAY_AREA_LEFT_SIDE_X_OFFSET + ((pip - 12) * PIP_WIDTH)) : \
    (PLAY_AREA_RIGHT_SIDE_X_OFFSET + ((pip - 18) * PIP_WIDTH)))))

#define GET_PIP_Y(pip) \
    ((pip >= 0 && pip <= 11) ? (PLAY_AREA_TOP_OFFSET + BOARD_HEIGHT - PIP_HEIGHT - CHECKER_SIZE) : (PLAY_AREA_TOP_OFFSET))


#define MAX_TEXT_LENGTH 100
#define Z_BOARD 1
#define Z_PIP 2
#define Z_CHECKER 3
#define Z_DEBUGTEXT 4

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
 * @brief A Snippet is a bit of text that is drawn on the screen. It can be manipulated using
 * the various Snippet_* functions.
*/
typedef struct Snippet Snippet;

/**
 * @brief Snippet update functions are called every frame to update the Snippet's state.
 */
typedef void (*SnippetUpdate_fn)(Snippet* snippet, void* data);

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
 * @brief Create a new managed Snippet instance. Do not free Snippets directly, as they are managed by the ViewManager.
 * 
 * @param vm The ViewManager instance
 * @param font The SDL_Font to use for this Snippet
 * @param color The color to use for this Snippet
 * @param text The text to use for this Snippet. A copy will be created and freed when the Snippet is freed.
 * @param x The x coordinate of the Snippet
 * @param y The y coordinate of the Snippet
 * @param z The z coordinate of the Snippet
 * @param update_fn The update function to call every frame
 * @param update_data The data to pass to the update function
 */
int32_t VM_createSnippet(ViewManager* vm, TTF_Font* font, SDL_Color color, char* text, int32_t x, int32_t y, int32_t z, SnippetUpdate_fn update_fn, void* update_data);

/**
 * @brief Set the location of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setLocation(Snippet* snippet, int32_t x, int32_t y);

/**
 * @brief Set the text of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setText(Snippet* snippet, char* text);
