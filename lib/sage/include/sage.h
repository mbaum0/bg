/**
 * @file sage.h
 * @author Michael Baumgarten
 * @brief Simply-Another-Game-Engine (SAGE) header file
 */

#pragma once
#include "vmanager.h"
#include "mmanager.h"

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
 * @brief Step the SAGE engine forward one frame
 * 
 * @param s The SAGE engine
 */
void Sage_step(Sage* s);

/**
 * @brief Load a texture from a file
 * 
 * @param s The SAGE engine
 * @param path The path to the texture file
 * @return SDL_Texture* The loaded texture
 */
SDL_Texture* Sage_loadTexture(Sage* s, char* path);
