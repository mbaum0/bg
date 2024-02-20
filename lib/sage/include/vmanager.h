/**
 * @file vmanager.h
 * @author Michael Baumgarten
 * @brief ViewManager implementation for the game engine
 */
#pragma once
#include "events.h"
#include "snippet.h"
#include "sprite.h"
#include <SDL3/SDL.h>

/**
 * @brief ViewManager manages rendering Sprites to the screen.
 */
typedef struct ViewManager ViewManager;
struct ViewManager {
    SDL_Renderer* renderer;
    Sprite*** sprites;
    Snippet*** snippets;
    SDL_FRect viewport;
    bool sortSpriteZNextFrame;
};

/**
 * @brief Initialize a new ViewManager instance and returns a pointer to it. Must
 * be freed with VM_free.
 */
ViewManager* VM_init(SDL_Renderer* renderer);

void VM_setViewport(ViewManager* vm, SDL_FRect viewport);

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
 * @return Sint32 The id of the Sprite
 */
Sint32 VM_registerSprite(ViewManager* vm, Sprite* sprite);

/**
 * @brief Register a Snippet with the ViewManager. Do not free Snippets directly once registered, as they are managed by
 * the ViewManager.
 *
 * @param vm The ViewManager instance
 * @param snippet Snippet to register
 * @return Sint32 The id of the Snippet
 */
Sint32 VM_registerSnippet(ViewManager* vm, Snippet* snippet);

/**
 * @brief Sprite update functions are called every frame to update the Sprite's state.
 */
typedef void (*SpriteUpdate_fn)(ViewManager* vm, Sprite* sprite, void* object, void* context);

/**
 * @brief Snippet update functions are called every frame to update the Snippet's state.
 */
typedef void (*SnippetUpdate_fn)(ViewManager* vm, Snippet* snippet, void* data);

/**
 * @brief Sprite click functions are called when the Sprite is clicked.
 */
typedef void (*SpriteClick_fn)(ViewManager* vm, Sprite* sprite, void* object, void* context, Sint32 code);

/**
 * @brief Snippet click functions are called when the Snippet is clicked.
 */
typedef void (*SnippetClick_fn)(ViewManager* vm, Snippet* snippet, void* object, void* context, Sint32 code);

void Sprite_registerUpdateFn(Sprite* sprite, SpriteUpdate_fn update_fn, void* object, void* context);
void Sprite_registerClickFn(Sprite* sprite, SpriteClick_fn click_fn, void* object, void* context, Sint32 code);
void Snippet_registerUpdateFn(Snippet* snippet, SnippetUpdate_fn update_fn, void* data);
void Snippet_registerClickFn(Snippet* snippet, SnippetClick_fn click_fn, void* object, void* context, Sint32 code);

/**
 * @brief Update the z-value of a sprite
 */
void VM_setSpriteZ(ViewManager* vm, Sprite* s, Sint32 newZ);

/**
 * @brief Get the Sprite at the given coordinates
 *
 * @param vm The ViewManager instance
 * @param x The x coordinate
 * @param y The y coordinate
 *
 * @return Sprite* The Sprite at the given coordinates, or NULL if no Sprite is found
 */
Sprite* VM_findSpriteAtCoordinate(ViewManager* vm, Sint32 x, Sint32 y);

Snippet* VM_findSnippetAtCoordinate(ViewManager* vm, Sint32 x, Sint32 y);

Sprite* VM_findSpriteCollision(ViewManager* vm, Sprite* sprite);
