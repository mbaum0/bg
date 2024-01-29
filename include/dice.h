/**
 * @file dice.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#pragma once

#include <stdint.h>
typedef struct GameDie GameDie;

struct GameDie {
    int32_t value;
    int32_t index;
    int32_t side;
};

void createDiceSprites(GameDie* die1, GameDie* die2);
