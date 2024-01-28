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

#define FPS 60
#define MS_PER_FRAME (1000 / FPS)

typedef struct Sage Sage;

struct Sage {
  MediaManager* mm;
  ViewManager* vm;
  EventManager* em;
};

/**
 * @brief Initialize the SAGE engine
 * @param title The title of the game
 * @param winWidth The width of the window
 * @param winHeight The height of the window
 */
void Sage_init(char* title, int winWidth, int winHeight);

/**
 * @brief Destroy the SAGE engine and free all resources
 *
 * @param s The SAGE engine
 */
void Sage_destroy(void);

void Sage_step(void);

int Sage_handleEvent(SDL_Event *event);

/**
 * @brief Load a texture from a file
 *
 * @param s The SAGE engine
 * @param path The path to the texture file
 * @return SDL_Texture* The loaded texture
 */
SDL_Texture* Sage_loadTexture(char* path);

SDL_Texture* Sage_loadSVGTexture(char* path, int32_t width, int32_t height);

/**
 * @brief Load a font from a file
 * 
 * @param s The SAGE engine
 * @param path The path to the font file
 * @param size Font size
 * @return TTF_Font* 
 */
TTF_Font* Sage_loadFont(char* path, int32_t size);

/**
 * @brief Register a sprite with the SAGE engine
 *
 * @param s The SAGE engine
 * @param sprite Sprite to register
 * @return int32_t The id of the Sprite
 */
int32_t Sage_registerSprite(Sprite* sprite);

/**
 * @brief Register a snippet with the SAGE engine
 * 
 * @param s The SAGE engine
 * @param snippet Snippet to register
 */
void Sage_registerSnippet(Snippet* snippet);

/**
 * @brief Register a callback function for an event
 *
 * @param s The SAGE engine
 * @param eventType Type of event
 * @param callback_fn Callback function
 * @param data data to pass to the callback function
 */
void Sage_registerEventCallback(uint32_t eventType, EventCallback_fn callback_fn, void* data);

void Sage_setViewport(SDL_FRect viewport);
