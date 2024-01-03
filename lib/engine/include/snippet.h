/**
 * @file snippet.h
 * @author Michael Baumgarten
 * @brief Spinnet implementation for the game engine
 */
#pragma once
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL_ttf.h>
#include "vmanager.h"

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

/**
 * @brief Set the location of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setLocation(Snippet* snippet, int32_t x, int32_t y);

/**
 * @brief Set the text of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setText(Snippet* snippet, char* text);
