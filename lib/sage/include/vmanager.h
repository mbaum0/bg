/**
 * @file vmanager.h
 * @author Michael Baumgarten
 * @brief ViewManager implementation for the game engine
 */
#pragma once
#include "events.h"
#include "snippet.h"
#include "sprite.h"
#include <SDL.h>

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
 * @brief Register a Sprite with the ViewManager. Do not free Sprites directly once registered, as they are managed by
 * the ViewManager.
 *
 * @param vm The ViewManager instance
 * @param sprite Sprite to register
 * @return int32_t The id of the Sprite
 */
int32_t VM_registerSprite(ViewManager* vm, Sprite* sprite);

/**
 * @brief Register a Snippet with the ViewManager. Do not free Snippets directly once registered, as they are managed by
 * the ViewManager.
 *
 * @param vm The ViewManager instance
 * @param snippet Snippet to register
 * @return int32_t The id of the Snippet
 */
int32_t VM_registerSnippet(ViewManager* vm, Snippet* snippet);
