/**
 * @file dice.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#pragma once

#include <stdint.h>
typedef struct GameDie GameDie;
typedef enum  { DICE_SWAP, DICE_MOVE, DICE_NONE } DiceAnimation;

struct GameDie {
    int32_t value;
    int32_t index;
    int32_t side;
    DiceAnimation animation;
};

void createDiceSprites(GameDie* die1, GameDie* die2);
