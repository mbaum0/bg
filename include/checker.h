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
#define CHECKER_LEFT_X_OFFSET_NORMAL 0.124823943661972
#define CHECKER_RIGHT_OFFSET_NORMAL 0.823004694835681
#define CHECKER_TOP_Y_OFFSET_NORMAL 0.050224887556222
#define CHECKER_BOTTOM_Y_OFFSET_NORMAL 0.879310344827586


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
