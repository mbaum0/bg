/**
 * @file game.h
 * @author Michael Baumgarten
 * @brief 
 */
#pragma once
#include "checker.h"
#include "dice.h"

typedef struct GameBoard GameBoard;

struct GameBoard {
  Checker lightCheckers[15];
  Checker darkCheckers[15];
  int32_t pipCounts[30];     // number of checkers on each pip
  GameDie die1;
  GameDie die2;
};

GameBoard* GameBoard_create(void);
