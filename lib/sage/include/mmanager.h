/**
 * @file mmanager.h
 * @author Michael Baumgarten
 * @brief Media Manager implementation for the game engine
 */

#pragma once
#include "array.h"
#include "log.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

typedef struct MediaManager MediaManager;

ARRAY_DEFINE(Texture, SDL_Texture)
ARRAY_DEFINE(Font, TTF_Font)

struct MediaManager {
  SDL_Renderer* renderer;
  SDL_Window* window;
  TextureArray textures;
  FontArray fonts;
};

/**
 * @brief Initializes the renderer, window, fonts and textures
 * @param title Title of the window
 * @param width Width of the window
 * @param height Height of the window
 * @return MediaManager instance
 */
MediaManager* MM_init(char* title, int32_t width, int32_t height);

/**
 * @brief Cleans up all initialized game media
 *
 * @param mm MediaManager instance to destroy
 */
void MM_free(MediaManager* mm);

/**
 * @brief Loads a texture from a file
 * @param mm MediaManager instance
 * @param path Path to texture file
 * @return SDL_Texture* pointer to the texture, NULL if failed to load
 */
SDL_Texture* MM_loadTexture(MediaManager* mm, char* path);

/**
 * @brief Loads a font from a file
 *
 * @param mm MediaManager instance
 * @param path Path to font file
 * @param size font size
 * @return TTF_Font* pointer to the font, NULL if failed to load
 */
TTF_Font* MM_loadFont(MediaManager* mm, char* path, int32_t size);
