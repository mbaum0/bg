/**
 * @file checker.h
 * @author Michael Baumgarten
 * @brief
 */
#pragma once
#include <stdint.h>
#include "sage.h"
#include "board.h"

typedef struct Checker Checker;
typedef struct GameBoard GameBoard;

struct Checker {
  int32_t pipIndex;
  int32_t pipOffset;
};

struct GameBoard {
  Checker lightCheckers[15];
  Checker darkCheckers[15];
  int32_t pipCounts[30];     // number of checkers on each pip
  int32_t die1;
  int32_t die2;
};

GameBoard* GameBoard_create(void);

void GameBoard_destroy(GameBoard* board);
