/**
 * @file mmanager.h
 * @author Michael Baumgarten
 * @brief Media Manager implementation for the game engine
 */

#pragma once
#include "font.h"
#include "log.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>

typedef struct SageTextureEntry SageTextureEntry;
struct SageTextureEntry {
    size_t key;
    SDL_Texture* value;
};
typedef struct SageFontEntry SageFontEntry;
struct SageFontEntry {
    size_t key;
    SageFont* value;
};

typedef struct MediaManager MediaManager;

struct MediaManager {
    SDL_Renderer* renderer;
    SDL_Window* window;
    SageTextureEntry* textures;
    SageFontEntry* fonts;
    float pixelScale;
};

/**
 * @brief Initializes the renderer, window, fonts and textures
 * @param title Title of the window
 * @param winWidth The width of the window
 * @param winHeight The height of the window
 * @return MediaManager instance
 */
MediaManager* MM_init(char* title, int winWidth, int winHeight);

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

SDL_Texture* MM_loadSVGTexture(MediaManager* mm, char* path, Sint32 width, Sint32 height);

/**
 * @brief Load a Bitmap font as a SageFont and returns it.
 * These are expected to be generated using fontbm from
 * github.com/vladimirgamalyan/fontbm
 *
 * @param mm MediaManager instance
 * @param imagePath Path to the .png font bitmap
 * @param formatPath Path to the font format file
 * @param srcSize pt size of the source font
 * @param dstSize desired pt rendering size
 * @return Sage_Font*
 */
SageFont* MM_loadBitmapFont(MediaManager* mm, char* imagePath, char* formatPath, Uint32 srcSize, Uint32 dstSize);
