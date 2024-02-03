/**
 * @file game.h
 * @author Michael Baumgarten
 * @brief 
 */
#pragma once
#include "checker.h"
#include "dice.h"

typedef struct GameBoard GameBoard;
typedef enum { LIGHT_DICE_ROLL, LIGHT_MOVE_ONE, LIGHT_MOVE_TWO, DARK_DICE_ROLL, DARK_MOVE_ONE, DARK_MOVE_TWO } GameState;

#define CHECKER_IS_BARRED(c) ((c->color == DARK && c->pipIndex == DARK_BAR) || (c->color == LIGHT && c->pipIndex == LIGHT_BAR))

struct GameBoard {
  Checker lightCheckers[15];
  Checker darkCheckers[15];
  int32_t pipCounts[28];
  GameDie die1;
  GameDie die2;
  GameState state;
};

GameBoard* GameBoard_create(void);
