/**
 * @file dstats.h
 * @author Michael Baumgarten
 * @brief Debug stats for the game
 */
#pragma once
#include "sage.h"
#include "checker.h"

typedef struct DStats DStats;
struct DStats {
    float fps;
    float gameHeight;
    float gameWidth;
    float pipHeight;
    float pipWidth;
    float checkerHeight;
    float checkerWidth;
    float checkerRenderedHeight;
    float checkerRenderedWidth;
};

void DStats_create(void);
