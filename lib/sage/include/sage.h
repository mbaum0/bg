/**
 * @file sage.h
 * @author Michael Baumgarten
 * @brief Simply-Another-Game-Engine (SAGE) header file
 */

#pragma once
#include "events.h"
#include "input.h"
#include "mmanager.h"
#include "vmanager.h"

typedef struct Sage Sage;

struct Sage {
    MediaManager* mm;
    ViewManager* vm;
    EventManager* em;
    Uint64 ticks;
};

/**
 * @brief Initialize the SAGE engine
 * @param title The title of the game
 * @param winWidth The width of the window
 * @param winHeight The height of the window
 * @param fillDisplay Whether size the window to size of the display
 */
void Sage_init(char* title, int winWidth, int winHeight, bool fillDisplay);

/**
 * @brief Destroy the SAGE engine and free all resources
 */
void Sage_destroy(void);

void Sage_step(void);

int Sage_handleEvent(SDL_Event* event);

/**
 * @brief Load a texture from a file
 *
 * @param path The path to the texture file
 * @return SDL_Texture* The loaded texture
 */
SDL_Texture* Sage_loadTexture(char* path);

SDL_Texture* Sage_loadSVGTexture(char* path, Sint32 width, Sint32 height);

float Sage_convertHighDPI(float value);

/**
 * @brief Load a Bitmap font as a SageFont and returns it.
 * These are expected to be generated using fontbm from
 * github.com/vladimirgamalyan/fontbm
 * 
 * @param imagePath Path to the .png font bitmap
 * @param formatPath Path to the font format file
 * @param srcSize pt size of the source font
 * @param dstSize desired pt rendering size
 * @return Sage_Font* 
 */
SageFont* Sage_loadBitmapFont(char* imagePath, char* formatPath, Uint32 srcSize, Uint32 dstSize);

/**
 * @brief Register a sprite with the SAGE engine
 *
 * @param sprite Sprite to register
 * @return Sint32 The id of the Sprite
 */
Sint32 Sage_registerSprite(Sprite* sprite);

/**
 * @brief Register a snippet with the SAGE engine
 *
 * @param snippet Snippet to register
 */
void Sage_registerSnippet(Snippet* snippet);

/**
 * @brief Register a callback function for an event
 *
 * @param eventType Type of event
 * @param callback_fn Callback function
 * @param data data to pass to the callback function
 */
void Sage_registerEventCallback(Uint32 eventType, EventCallback_fn callback_fn, void* data);

void Sage_setViewport(SDL_FRect viewport);

/**
 * @brief Set the z-value of the sprite
 */
void Sprite_setZ(Sprite* s, Sint32 newZ);
