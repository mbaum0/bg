/**
 * @file dice.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#pragma once

#include <stdbool.h>

#include <SDL3/SDL.h>

typedef struct GameDie GameDie;
typedef enum { DICE_SWAP, DICE_MOVE, DICE_NONE } DiceAnimation;

struct GameDie {
    Sint32 value;
    Sint32 index;
    Sint32 side;
    Sint32 uses; // int instead of bool to account for doubles
    DiceAnimation animation;
};

void createDiceSprites(GameDie* die1, GameDie* die2);
