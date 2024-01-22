/**
 * @file checker.h
 * @author Michael Baumgarten
 * @brief
 */
#pragma once
#include <stdint.h>
#include "sage.h"
#include "board.h"

#define CHECKER_SRC_SIZE 188
#define CHECKER_W_NORMAL 0.055164319248826
#define CHECKER_H_NORMAL 0.070464767616192
//#define CHECKER_H_OFFSET_NORMAL (CHECKER_H_NORMAL * .95)
#define CHECKER_H_OFFSET_NORMAL 0.066941529235382
#define CHECKER_LEFT_X_OFFSET_NORMAL 0.124823943661972
#define CHECKER_RIGHT_OFFSET_NORMAL 0.823004694835681
#define CHECKER_TOP_Y_OFFSET_NORMAL 0.050224887556222
#define CHECKER_BOTTOM_Y_OFFSET_NORMAL 0.879310344827586

#define CHECKER_VELOCITY 0.08
#define CHECKER_ACCELERATION 0.001

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
