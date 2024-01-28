/**
 * @file mmanager.c
 * @author Michael Baumgarten
 * @brief Media Manager implementation for the game engine
 */
#include "mmanager.h"
#include "stb_ds.h"

bool initSDL(MediaManager* mm, char* title, int winWidth, int winHeight, bool fillDisplay) {
  (void)winHeight;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    log_error("Couldn't initialize SDL: %s", SDL_GetError());
    return false;
  }
  int w, h;

  if (fillDisplay){
    SDL_DisplayID did = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode* dm = SDL_GetCurrentDisplayMode(did);
    w = dm->w;
    h = dm->h;
    mm->window = SDL_CreateWindow(title, w, h, SDL_WINDOW_HIGH_PIXEL_DENSITY);
    mm->pixelScale = (dm->pixel_density * w) / (float)winWidth;
  } else {
    mm->window = SDL_CreateWindow(title, winWidth, winHeight, SDL_WINDOW_HIGH_PIXEL_DENSITY);
    mm->pixelScale = 1;
  }

  if (!mm->window) {
    log_error("Failed to open %d x %d window: %s", w, h, SDL_GetError());
    return false;
  }

  mm->renderer = SDL_CreateRenderer(mm->window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  //SDL_SetRenderLogicalPresentation(mm->renderer, 430, 932, SDL_LOGICAL_PRESENTATION_LETTERBOX, SDL_SCALEMODE_BEST);

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

MediaManager* MM_init(char* title, int winWidth, int winHeight, bool fillDisplay) {
  MediaManager* mm = calloc(1, sizeof(MediaManager));
  if (!initSDL(mm, title, winWidth, winHeight, fillDisplay)) {
    log_error("Failed to initialize SDL");
    return NULL;
  }
  mm->textures = malloc(sizeof(SDL_Texture**));
  mm->fonts = malloc(sizeof(TTF_Font**));
  return mm;
}

void destroySDL(MediaManager* mm) {
  SDL_DestroyRenderer(mm->renderer);
  SDL_DestroyWindow(mm->window);
  SDL_Quit();
}

void destroyTextures(MediaManager* mm) {
  SDL_Texture** textures = *mm->textures;
  for (int32_t i = 0; i < arrlen(textures); i++) {
    SDL_DestroyTexture(textures[i]);
  }
  IMG_Quit();
}

void destroyFonts(MediaManager* mm) {
  TTF_Font** fonts = *mm->fonts;
  for (int32_t i = 0; i < arrlen(fonts); i++) {
    TTF_CloseFont(fonts[i]);
  }
  TTF_Quit();
}

void MM_free(MediaManager* mm) {
  destroyFonts(mm);
  destroyTextures(mm);
  arrfree(*mm->textures);
  free(mm->textures);
  arrfree(*mm->fonts);
  free(mm->fonts);
  destroySDL(mm);
  free(mm);
}

SDL_Texture* MM_loadTexture(MediaManager* mm, char* path) {
  log_debug("Loading texture: %s", path);
  SDL_Texture* texture = IMG_LoadTexture(mm->renderer, path);
  if (!texture) {
    log_error("Failed to load texture: %s", IMG_GetError());
    return NULL;
  }
  arrput(*mm->textures, texture);
  return texture;
}

SDL_Texture* MM_loadSVGTexture(MediaManager* mm, char* path, int32_t width, int32_t height) {
  SDL_RWops* f = SDL_RWFromFile(path, "rb");
  SDL_Surface* s = IMG_LoadSizedSVG_RW(f, width, height);
  SDL_Texture* t = SDL_CreateTextureFromSurface(mm->renderer, s);
  SDL_DestroySurface(s);
  return t;
}

TTF_Font* MM_loadFont(MediaManager* mm, char* path, int32_t size) {
  log_debug("Loading font: %s", path);
  TTF_Font* font = TTF_OpenFont(path, size);
  if (!font) {
    log_error("Failed to load font: %s", TTF_GetError());
    return NULL;
  }
  arrput(*mm->fonts, font);
  return font;
}
