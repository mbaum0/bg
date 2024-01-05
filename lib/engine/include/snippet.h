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

/**
 * @brief A Snippet is a bit of text that is drawn on the screen. It can be manipulated using
 * the various Snippet_* functions.
*/
typedef struct Snippet Snippet;

typedef struct SnippetArray SnippetArray;

/**
 * @brief Snippet update functions are called every frame to update the Snippet's state.
 */
typedef void (*SnippetUpdate_fn)(Snippet* snippet, void* data);

struct Snippet {
    int32_t id;
    TTF_Font* font;
    SDL_Color color;
    int32_t x;
    int32_t y;
    int32_t z;
    bool visible;
    char* text;
    SDL_Rect dst_rect;
    SDL_Texture* texture;
    SnippetUpdate_fn update_fn;
    void* update_data;
    SDL_Renderer* renderer; // Snippets get a special pointer to the renderer so they can update their texture
};

struct SnippetArray {
    Snippet** snippets;
    int32_t size;
};

/**
 * @brief Initialize a SnippetArray
 */
void SnippetArray_init(SnippetArray* snippetArray);

/**
 * @brief Append a Snippet to a SnippetArray
 */
void SnippetArray_append(SnippetArray* snippetArray, Snippet* snippet);

/**
 * @brief Free a SnippetArray
 */
void SnippetArray_free(SnippetArray* snippetArray);

/**
 * @brief Create a new Snippet instance.
 * 
 * @param font The SDL_Font to use for this Snippet
 * @param color Color of text
 * @param text String text. A copy will be created and freed when the Snippet is freed
 * @param x The x coordinate
 * @param y The y coordinate
 * @param z The z coordinate
 * @param visible Whether or not the Snippet is visible
 * @param update_fn The update function to call every frame
 * @param update_data The data to pass to the update function
 * @return Snippet* 
 */
Snippet* Snippet_create(TTF_Font* font, SDL_Color color, char* text, int32_t x, int32_t y, int32_t z, bool visible, SnippetUpdate_fn update_fn, void* update_data);

/**
 * @brief Set the location of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setLocation(Snippet* snippet, int32_t x, int32_t y);

/**
 * @brief Set the text of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setText(Snippet* snippet, char* text);
