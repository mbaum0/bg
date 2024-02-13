/**
 * @file dice.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
typedef struct GameDie GameDie;
typedef enum  { DICE_SWAP, DICE_MOVE, DICE_NONE } DiceAnimation;

struct GameDie {
    int32_t value;
    int32_t index;
    int32_t side;
    int32_t uses; // int instead of bool to account for doubles
    DiceAnimation animation;
};

void createDiceSprites(GameDie* die1, GameDie* die2);
