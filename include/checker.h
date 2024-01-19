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
#define CHECKER_BOARD_WIDTH_PCT .05987
#define CHECKER_BOARD_X_LEFT_OFFSET_PCT .09108
#define CHECKER_BOARD_X_RIGHT_OFFSET_PCT .15095

typedef struct Checker Checker;

typedef enum Player {
  P_LIGHT,
  P_DARK,
} Player;

struct Checker {
  int32_t pipIndex;
  Player player;
};

Checker* Checker_create(Sage* sage, int32_t pipIndex, Player player);

void Checker_destroy(Checker* c);
