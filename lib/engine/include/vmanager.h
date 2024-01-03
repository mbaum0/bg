/**
 * @file vmanager.h
 * @author Michael Baumgarten
 * @brief ViewManager implementation for the game engine
 */
#pragma once
#include <SDL.h>
#include "sprite.h"
#include "snippet.h"

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
int32_t VM_createSnippet(ViewManager* vm, TTF_Font* font, SDL_Color color, char* text, int32_t x, int32_t y, int32_t z, bool visible, SnippetUpdate_fn update_fn, void* update_data);
