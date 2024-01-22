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
  int width;
  int height;
};

/**
 * @brief Create and initialize the SAGE engine
 * @param title The title of the game
 * @param width The width of the window
 * @param height The height of the window
 *
 * @return Sage* The new SAGE engine
 */
Sage* Sage_create(char* title, int width, int height);

/**
 * @brief Set the normalization rect on the view manager. This is used to normalize the coordinate system for sprites.
 * The default is (0, 0, 0, 0), which means that the coordinate system is not normalized.
 * @param s 
 * @param rect 
 */
void Sage_setNormalRect(Sage* s, SDL_FRect rect);

/**
 * @brief Destroy the SAGE engine and free all resources
 *
 * @param s The SAGE engine
 */
void Sage_destroy(Sage* s);

/**
 * @brief Run the SAGE engine. This function will block until the user exits the game.
 *
 * @param s The SAGE engine
 */
void Sage_run(Sage* s);

/**
 * @brief Load a texture from a file
 *
 * @param s The SAGE engine
 * @param path The path to the texture file
 * @return SDL_Texture* The loaded texture
 */
SDL_Texture* Sage_loadTexture(Sage* s, char* path);

/**
 * @brief Load a font from a file
 * 
 * @param s The SAGE engine
 * @param path The path to the font file
 * @param size Font size
 * @return TTF_Font* 
 */
TTF_Font* Sage_loadFont(Sage* s, char* path, int32_t size);

/**
 * @brief Register a sprite with the SAGE engine
 *
 * @param s The SAGE engine
 * @param sprite Sprite to register
 * @return int32_t The id of the Sprite
 */
int32_t Sage_registerSprite(Sage* s, Sprite* sprite);

/**
 * @brief Register a snippet with the SAGE engine
 * 
 * @param s The SAGE engine
 * @param snippet Snippet to register
 */
void Sage_registerSnippet(Sage* s, Snippet* snippet);

/**
 * @brief Register a callback function for an event
 *
 * @param s The SAGE engine
 * @param eventType Type of event
 * @param callback_fn Callback function
 * @param data data to pass to the callback function
 */
void Sage_registerEventCallback(Sage* s, uint32_t eventType, EventCallback_fn callback_fn, void* data);
