/**
 * @file dialog.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct Dialog Dialog;

struct Dialog {
    bool visible;
    bool moving;
    Uint32 alpha;
    bool playerWon;
    Uint32 maxGames;
    Uint32 roundsWon;
    Uint32 roundsLost;
    float x;
    float y;
};

void createDialogSprite(Dialog* d);
