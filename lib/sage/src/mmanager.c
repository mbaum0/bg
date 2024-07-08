/**
 * @file mmanager.c
 * @author Michael Baumgarten
 * @brief Media Manager implementation for the game engine
 */
#include "mmanager.h"
#include "stb_ds.h"

bool initSDL(MediaManager* mm, char* title, int winWidth, int winHeight) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        log_error("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }
    int w = winWidth;
    int h = winHeight;
    mm->pixelScale = 1;
    int windowFlags = 0;

    if (TARGET_OS_IOS) {
        SDL_DisplayID did = SDL_GetPrimaryDisplay();
        const SDL_DisplayMode* dm = SDL_GetCurrentDisplayMode(did);
        w = dm->w;
        h = dm->h;
        windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
        mm->pixelScale = (dm->pixel_density * w) / (float)winWidth;
    }

    mm->window = SDL_CreateWindow(title, w, h, windowFlags);
    if (!mm->window) {
        log_error("Failed to open %d x %d window: %s", w, h, SDL_GetError());
        return false;
    }

    mm->renderer = SDL_CreateRenderer(mm->window, NULL);
    SDL_SetRenderVSync(mm->renderer, 1);

    if (!mm->renderer) {
        log_error("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        log_error("Couldn't initialize SDL_image: %s", IMG_GetError());
        return false;
    }

    return true;
}

MediaManager* MM_init(char* title, int winWidth, int winHeight) {
    MediaManager* mm = SDL_calloc(1, sizeof(MediaManager));
    if (mm == NULL) {
        log_error("Failed to allocate memory for the media manager");
        return NULL;
    }
    if (!initSDL(mm, title, winWidth, winHeight)) {
        log_error("Failed to initialize SDL");
        free(mm);
        return NULL;
    }
    hmdefault(mm->textures, NULL);
    hmdefault(mm->fonts, NULL);
    return mm;
}

void destroySDL(MediaManager* mm) {
    SDL_DestroyRenderer(mm->renderer);
    SDL_DestroyWindow(mm->window);
    SDL_Quit();
}

void destroyTextures(MediaManager* mm) {
    for (Sint32 i = 0; i < hmlen(mm->textures); i++) {
        SDL_DestroyTexture(mm->textures[i].value);
    }
    IMG_Quit();
}

void destroyFonts(MediaManager* mm) {
    for (Sint32 i = 0; i < hmlen(mm->fonts); i++) {
        BMFont_DestroyFont(&mm->fonts[i].value->layout);
    }
}

void MM_free(MediaManager* mm) {
    destroyFonts(mm);
    shfree(mm->fonts);
    destroyTextures(mm);
    shfree(mm->textures);
    destroySDL(mm);
    free(mm);
}

SDL_Texture* MM_loadTexture(MediaManager* mm, char* path) {
    size_t hash = stbds_hash_string(path, 0xFEED);
    SDL_Texture* texture = hmget(mm->textures, hash);
    if (texture != NULL) {
        return texture;
    }
    log_debug("Loading texture: %s", path);
    texture = IMG_LoadTexture(mm->renderer, path);
    if (!texture) {
        log_error("Failed to load texture: %s", IMG_GetError());
        return NULL;
    }

    hmput(mm->textures, hash, texture);
    return texture;
}

SDL_Texture* MM_loadSVGTexture(MediaManager* mm, char* path, Sint32 width, Sint32 height) {
    SDL_IOStream* f = SDL_IOFromFile(path, "rb");
    SDL_Surface* s = IMG_LoadSizedSVG_IO(f, width, height);
    SDL_CloseIO(f);
    SDL_Texture* t = SDL_CreateTextureFromSurface(mm->renderer, s);
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(s);
    return t;
}

SageFont* MM_loadBitmapFont(MediaManager* mm, char* imagePath, char* formatPath, Uint32 srcSize, Uint32 dstSize) {
    size_t hash = stbds_hash_string(imagePath, 0xFEED);
    hash += stbds_hash_string(formatPath, 0xFEED);
    SageFont* font = hmget(mm->fonts, hash);
    if (font != NULL) {
        return font;
    }
    font = malloc(sizeof(SageFont));
    log_debug("Loading font: %s", imagePath);
    if (BMFont_OpenFont(formatPath, &font->layout)) {
        log_error("Failed to load font formatting");
        free(font);
        return NULL;
    }
    font->texture = MM_loadTexture(mm, imagePath);
    font->scale = mm->pixelScale;
    font->srcSize = srcSize;
    font->dstSize = dstSize;
    return font;
}
