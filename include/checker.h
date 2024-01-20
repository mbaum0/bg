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
#define CHECKER_H_OFFSET_NORMAL (CHECKER_H_NORMAL * .95)
#define CHECKER_LEFT_X_OFFSET_NORMAL 0.124823943661972
#define CHECKER_RIGHT_OFFSET_NORMAL 0.823004694835681
#define CHECKER_TOP_Y_OFFSET_NORMAL 0.050224887556222
#define CHECKER_BOTTOM_Y_OFFSET_NORMAL 0.879310344827586

#define CHECKER_VELOCITY 0.08
#define CHECKER_ACCELERATION 0.001

typedef struct Checker Checker;
typedef struct Checkers Checkers;

typedef enum Player {
  P_LIGHT,
  P_DARK,
} Player;

struct Checker {
  int32_t pipIndex;
  int32_t pipOffset;
  float xVelocity;
  float yVelocity;
  Player player;
};

struct Checkers {
  Checker lightCheckers[15];
  Checker darkCheckers[15];
};

//Checker* Checker_create(Sage* sage, int32_t pipIndex, Player player);
Checkers* Checkers_create(Sage* sage);

void Checker_destroy(Checker* c);
