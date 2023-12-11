#include "media.h"

bool initSDL(MediaManager *mm);
bool loadFonts(MediaManager *mm);
void initColors(MediaManager *mm);
bool loadTexture(SDL_Texture **dst, MediaManager *mm, char *filename);
bool loadTextures(MediaManager *mm);
void destroyTextures(MediaManager *mm);
void destroyFonts(MediaManager *mm);
void destroySDL(MediaManager *mm);

bool initSDL(MediaManager *mm) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    mm->window = SDL_CreateWindow("Backgammon!", SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                                         WINDOW_HEIGHT, 0);

    if (!mm->window) {
        log_error("Failed to open %d x %d window: %s", WINDOW_WIDTH,
                  WINDOW_HEIGHT, SDL_GetError());
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

bool loadFonts(MediaManager *mm) {
    mm->fonts.debug =
        TTF_OpenFont("assets/VeraMono.ttf", 16);
    if (!mm->fonts.debug) {
        log_error("Failed to open font: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void initColors(MediaManager *mm) {
    mm->colors.white = (SDL_Color){255, 255, 255, 255};
}

bool loadTexture(SDL_Texture **dst, MediaManager *mm, char *filename) {
    log_debug("Loading texture: %s", filename);
    SDL_Texture *texture;
    texture = IMG_LoadTexture(mm->renderer, filename);

    *dst = texture;
    if (texture == NULL) {
        log_error("Failed to load texture: %s", IMG_GetError());
        return false;
    }
    return true;
}

bool loadTextures(MediaManager *mm) {
    return (
        loadTexture(&(mm->textures.board), mm, "assets/board.png") &&
        loadTexture(&(mm->textures.checker), mm, "assets/checker.png") &&
        loadTexture(&(mm->textures.dice), mm, "assets/dice.png") &&
        loadTexture(&(mm->textures.rollBtn), mm, "assets/rollBtn.png") &&
        loadTexture(&(mm->textures.pip), mm, "assets/pip.png") &&
        loadTexture(&(mm->textures.confirmBtn), mm, "assets/confirmBtn.png") &&
        loadTexture(&(mm->textures.undoBtn), mm, "assets/undoBtn.png")
        );
}

void destroyTextures(MediaManager *mm) {
    SDL_DestroyTexture(mm->textures.board);
    SDL_DestroyTexture(mm->textures.checker);
    SDL_DestroyTexture(mm->textures.dice);
    SDL_DestroyTexture(mm->textures.rollBtn);
    SDL_DestroyTexture(mm->textures.pip);
    SDL_DestroyTexture(mm->textures.confirmBtn);
    SDL_DestroyTexture(mm->textures.undoBtn);
    IMG_Quit();
}

void destroyFonts(MediaManager *mm) {
    TTF_CloseFont(mm->fonts.debug);
    TTF_Quit();
}

void destroySDL(MediaManager *mm) {
    SDL_DestroyRenderer(mm->renderer);
    SDL_DestroyWindow(mm->window);
    SDL_Quit();
}

MediaManager* MM_init(void) {
    MediaManager *mm = calloc(1, sizeof(MediaManager));
    bool res =
        (initSDL(mm) && loadTextures(mm) && loadFonts(mm));
    initColors(mm);
    if (!res) {
        MM_free(mm);
        return NULL;
    }
    return mm;
}

void MM_free(MediaManager *mm) {
    destroyTextures(mm);
    destroyFonts(mm);
    destroySDL(mm);
    free(mm);
}
