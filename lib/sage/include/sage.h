/**
 * @file sage.h
 * @author Michael Baumgarten
 * @brief Simply-Another-Game-Engine (SAGE) header file
 */

#pragma once
#include "vmanager.h"
#include "mmanager.h"
#include "input.h"

#define FPS 60
#define MS_PER_FRAME (1000 / FPS)

typedef struct Sage Sage;

struct Sage {
    MediaManager* mm;
    ViewManager* vm;
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
 * @brief Register a sprite with the SAGE engine
 * 
 * @param s The SAGE engine
 * @param sprite Sprite to register
 * @return int32_t The id of the Sprite
 */
int32_t Sage_registerSprite(Sage* s, Sprite* sprite);
