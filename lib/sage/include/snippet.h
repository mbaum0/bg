/**
 * @file snippet.h
 * @author Michael Baumgarten
 * @brief Spinnet implementation for the game engine
 */
#pragma once
#include "font.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
    char c;
    SDL_FRect src; // where in the texture this char is located
    SDL_FRect dst;
} SnippetChar;

/**
 * @brief A Snippet is a bit of text that is drawn on the screen. It can be manipulated using
 * the various Snippet_* functions.
 */
typedef struct Snippet Snippet;

struct Snippet {
    Sint32 id;
    SageFont* font;
    Sint32 x;
    Sint32 y;
    Sint32 z;
    bool visible;
    char* text;
    SnippetChar* chars;
    Sint32 textLen;
    SDL_FRect boundBox;
    SDL_Color color;
    void* update_fn;
    void* update_data;
    void* click_fn;
    void* click_object;
    void* click_context;
    Sint32 click_code;
    bool useViewport;
};

/**
 * @brief Create a new Snippet instance.
 *
 * @param font The SDL_Font to use for this Snippet
 * @param x The x coordinate
 * @param y The y coordinate
 * @param z The z coordinate
 * @param visible Whether or not the Snippet is visible
 * @return Snippet*
 */
Snippet* Snippet_create(SageFont* font, SDL_Color color, Sint32 x, Sint32 y, Sint32 z, bool visible);
/**
 * @brief Set the location of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setLocation(Snippet* snippet, Sint32 x, Sint32 y);

/**
 * @brief Set the text of the Snippet. Only used in SnippetUpdate_fn callbacks
 */
void Snippet_setText(Snippet* snippet, char* text);
