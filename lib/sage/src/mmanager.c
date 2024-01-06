/**
 * @file mmanager.c
 * @author Michael Baumgarten
 * @brief Media Manager implementation for the game engine
 */
#include "mmanager.h"

ARRAY_INIT(Texture, SDL_Texture)
ARRAY_INIT(Font, TTF_Font)

bool initSDL(MediaManager* mm, char* title, int32_t width, int32_t height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    mm->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED, width,
                                         height, 0);

    if (!mm->window) {
        log_error("Failed to open %d x %d window: %s", width,
                  height, SDL_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    mm->renderer =
        SDL_CreateRenderer(mm->window, -1, 0);

    if (!mm->renderer) {
        log_error("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() < 0) {
        log_error("Couldn't initialize SDL_ttf: %s", TTF_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        log_error("Couldn't initialize SDL_image: %s", IMG_GetError());
        return false;
    }

    return true;
}

MediaManager* MM_init(char* title, int32_t width, int32_t height){
    MediaManager* mm = calloc(1, sizeof(MediaManager));
    TextureArray_init(&mm->textures, 10);
    FontArray_init(&mm->fonts, 10);
    if (!initSDL(mm, title, width, height)) {
        log_error("Failed to initialize SDL");
        return NULL;
    }
    return mm;
}

void destroySDL(MediaManager *mm) {
    SDL_DestroyRenderer(mm->renderer);
    SDL_DestroyWindow(mm->window);
    SDL_Quit();
}

void destroyTextures(MediaManager* mm){
    int32_t i;
    SDL_Texture* texture;
    while ((texture = TextureArray_iterator(&mm->textures, &i)) != NULL) {
        SDL_DestroyTexture(texture);
    }
    IMG_Quit();
}

void destroyFonts(MediaManager* mm){
    int32_t i;
    TTF_Font* font;
    while ((font = FontArray_iterator(&mm->fonts, &i)) != NULL) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
}

void MM_free(MediaManager* mm){
    destroyFonts(mm);
    destroyTextures(mm);
    TextureArray_free(&mm->textures);
    FontArray_free(&mm->fonts);
    destroySDL(mm);
    free(mm);
}

SDL_Texture* MM_loadTexture(MediaManager* mm, char* path){
    log_debug("Loading texture: %s", path);
    SDL_Texture* texture = IMG_LoadTexture(mm->renderer, path);
    if (!texture) {
        log_error("Failed to load texture: %s", IMG_GetError());
        return NULL;
    }
    TextureArray_append(&mm->textures, texture);
    return texture;
}

TTF_Font* MM_loadFont(MediaManager* mm, char* path, int32_t size){
    log_debug("Loading font: %s", path);
    TTF_Font* font = TTF_OpenFont(path, size);
    if (!font) {
        log_error("Failed to load font: %s", TTF_GetError());
        return NULL;
    }
    FontArray_append(&mm->fonts, font);
    return font;
}
