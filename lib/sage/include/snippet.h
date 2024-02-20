/**
 * @file snippet.h
 * @author Michael Baumgarten
 * @brief Spinnet implementation for the game engine
 */
#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>

/**
 * @brief A Snippet is a bit of text that is drawn on the screen. It can be manipulated using
 * the various Snippet_* functions.
 */
typedef struct Snippet Snippet;

struct Snippet {
    Sint32 id;
    TTF_Font* font;
    SDL_Color color;
    Sint32 x;
    Sint32 y;
    Sint32 z;
    bool visible;
    char* text;
    SDL_FRect dst_rect;
    SDL_Texture* texture;
    void* update_fn;
    void* update_data;
    void* click_fn;
    void* click_object;
    void* click_context;
    Sint32 click_code;
    SDL_Renderer* renderer; // Snippets get a special pointer to the renderer so they can update their texture
};

/**
 * @brief Create a new Snippet instance.
 *
 * @param font The SDL_Font to use for this Snippet
 * @param color Color of text
 * @param x The x coordinate
 * @param y The y coordinate
 * @param z The z coordinate
 * @param visible Whether or not the Snippet is visible
 * @return Snippet*
 */
Snippet* Snippet_create(TTF_Font* font, SDL_Color color, Sint32 x, Sint32 y, Sint32 z, bool visible);
/**
 * @brief Set the location of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setLocation(Snippet* snippet, Sint32 x, Sint32 y);

/**
 * @brief Set the text of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setText(Snippet* snippet, char* text);
