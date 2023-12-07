#include "media.h"

bool initSDL(MediaManager *mediaManager);
bool loadFonts(MediaManager *mediaManager);
void initColors(MediaManager *mediaManager);
bool loadTexture(SDL_Texture **dst, MediaManager *mediaManager, char *filename);
bool loadTextures(MediaManager *mediaManager);
void destroyTextures(MediaManager *mediaManager);
void destroyFonts(MediaManager *mediaManager);
void destroySDL(MediaManager *mediaManager);

bool initSDL(MediaManager *mediaManager) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    mediaManager->window = SDL_CreateWindow("Backgammon!", SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                                         WINDOW_HEIGHT, 0);

    if (!mediaManager->window) {
        log_error("Failed to open %d x %d window: %s", WINDOW_WIDTH,
                  WINDOW_HEIGHT, SDL_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    mediaManager->renderer =
        SDL_CreateRenderer(mediaManager->window, -1, 0);

    if (!mediaManager->renderer) {
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

bool loadFonts(MediaManager *mediaManager) {
    mediaManager->fonts.gameFont =
        TTF_OpenFont("assets/Commodore Rounded v1.2.ttf", 15);
    if (!mediaManager->fonts.gameFont) {
        log_error("Failed to open font: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void initColors(MediaManager *mediaManager) {
    mediaManager->colors.white = (SDL_Color){255, 255, 255, 255};
}

bool loadTexture(SDL_Texture **dst, MediaManager *mediaManager, char *filename) {
    log_debug("Loading texture: %s", filename);
    SDL_Texture *texture;
    texture = IMG_LoadTexture(mediaManager->renderer, filename);

    *dst = texture;
    if (texture == NULL) {
        log_error("Failed to load texture: %s", IMG_GetError());
        return false;
    }
    return true;
}

bool loadTextures(MediaManager *mediaManager) {
    return (
        loadTexture(&(mediaManager->textures.board), mediaManager, "assets/bck.png") &&
        loadTexture(&(mediaManager->textures.checker), mediaManager, "assets/checker.png") &&
        loadTexture(&(mediaManager->textures.dice), mediaManager, "assets/dice.png") &&
        loadTexture(&(mediaManager->textures.rollBtn), mediaManager, "assets/rollBtn.png")
        );
}

void destroyTextures(MediaManager *mediaManager) {
    SDL_DestroyTexture(mediaManager->textures.board);
    SDL_DestroyTexture(mediaManager->textures.checker);
    SDL_DestroyTexture(mediaManager->textures.dice);
    SDL_DestroyTexture(mediaManager->textures.rollBtn);
    IMG_Quit();
}

void destroyFonts(MediaManager *mediaManager) {
    TTF_CloseFont(mediaManager->fonts.gameFont);
    TTF_Quit();
}

void destroySDL(MediaManager *mediaManager) {
    SDL_DestroyRenderer(mediaManager->renderer);
    SDL_DestroyWindow(mediaManager->window);
    SDL_Quit();
}

MediaManager* media_init(void) {
    MediaManager *mediaManager = calloc(1, sizeof(MediaManager));
    bool res =
        (initSDL(mediaManager) && loadTextures(mediaManager) && loadFonts(mediaManager));
    initColors(mediaManager);
    if (!res) {
        media_free(mediaManager);
        return NULL;
    }
    return mediaManager;
}

void media_free(MediaManager *mediaManager) {
    destroyTextures(mediaManager);
    destroyFonts(mediaManager);
    destroySDL(mediaManager);
}
