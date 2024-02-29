/**
 * @file dialog.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#pragma once
#include <stdbool.h>
#include <SDL3/SDL.h>

typedef struct Dialog Dialog;

struct Dialog {
    bool visible;
    bool playerWon;
    Uint32 maxGames;
    Uint32 matchesWon;
    Uint32 matchesLost;
};

void createDialogSprite(Dialog* d);
