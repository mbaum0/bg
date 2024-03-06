/**
 * @file font.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#pragma once
#include "bmfont.h"
#include <SDL3/SDL.h>

typedef struct {
    SDL_Texture* texture;
    BMFont layout;
    float scale; // how much to scale this font by when rendering. provided by the media manager
    Uint32 srcSize;
    Uint32 dstSize;
} SageFont;

#define NORMALIZE_FONT(x, src, dst) (((float)x / (float)src) * (float)dst)
