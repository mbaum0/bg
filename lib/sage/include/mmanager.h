/**
 * @file mmanager.h
 * @author Michael Baumgarten
 * @brief Media Manager implementation for the game engine
 */

#pragma once
#include "log.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>

typedef struct MediaManager MediaManager;

struct MediaManager {
  SDL_Renderer* renderer;
  SDL_Window* window;
  SDL_Texture*** textures;
  TTF_Font*** fonts;
};

/**
 * @brief Initializes the renderer, window, fonts and textures
 * @param title Title of the window
 * @param winWidth The width of the window
 * @param winHeight The height of the window
 * @param gameWidth The logical width of the game
 * @param gameHeight The logical height of the game
 * @return MediaManager instance
 */
MediaManager* MM_init(char* title, int winWidth, int winHeight, int gameWidth, int gameHeight);

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
