#pragma once
#include "log.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

typedef struct TexturePack TexturePack;
typedef struct FontPack FontPack;
typedef struct ColorPack ColorPack;
// typedef struct SoundPack SoundPack;
typedef struct MediaManager MediaManager;

struct TexturePack {
    SDL_Texture* board;
    SDL_Texture* checker;
    SDL_Texture* dice;
    SDL_Texture* rollBtn;
    SDL_Texture* pip;
    SDL_Texture* confirmBtn;
    SDL_Texture* undoBtn;
};

struct FontPack {
    TTF_Font* debug;
    TTF_Font* standard;
};

struct ColorPack {
    SDL_Color white;
};

struct MediaManager {
    SDL_Renderer* renderer;
    SDL_Window* window;
    TexturePack textures;
    FontPack fonts;
    ColorPack colors;
};

/**
 * @brief Initializes the renderer, window, fonts and textures
 */
MediaManager* MM_init(void);

/**
 * @brief Cleans up all initialized game media
 *
 * @param mm MediaManager instance to destroy
 */
void MM_free(MediaManager* mm);
