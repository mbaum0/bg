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
 * @brief Set the location of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setLocation(Snippet* snippet, int32_t x, int32_t y);

/**
 * @brief Set the text of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setText(Snippet* snippet, char* text);
