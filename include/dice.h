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
    Sint32 value; // value on the face of the die
    Sint32 index; // 0 or 1, order of the dice on the board (swap when the user taps on dice)
    Sint32 side;  // side of the board the dice appear on
    Sint32 uses;  // int instead of bool to account for doubles
    DiceAnimation animation;
    bool moving;
};

void createDiceSprites(GameDie* die1, GameDie* die2);
