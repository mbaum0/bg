/**
 * @file game.h
 * @author Michael Baumgarten
 * @brief 
 */
#pragma once
#include "checker.h"

typedef struct GameBoard GameBoard;

struct GameBoard {
  Checker lightCheckers[15];
  Checker darkCheckers[15];
  int32_t pipCounts[30];     // number of checkers on each pip
  int32_t die1;
  int32_t die2;
};

GameBoard* GameBoard_create(void);
