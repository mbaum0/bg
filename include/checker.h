/**
 * @file checker.h
 * @author Michael Baumgarten
 * @brief
 */
#pragma once
#include "board.h"
#include "sage.h"

typedef struct Checker Checker;
typedef enum { LIGHT, DARK, NONE } Color;
typedef enum { PLAYER_ONE, PLAYER_TWO, PLAYER_NONE } Player;

#define PLAYER_ONE_HOME (0)
#define PLAYER_TWO_BAR (PLAYER_ONE_HOME)

#define PLAYER_ONE_BAR (25)
#define PLAYER_TWO_HOME (PLAYER_ONE_BAR)

#define IS_PLAYERS_HOME(player, pipIndex)                                                                              \
    ((player == PLAYER_ONE && pipIndex == PLAYER_ONE_HOME) || (player == PLAYER_TWO && pipIndex == PLAYER_TWO_HOME))

#define IS_CHECKER_BARRED(c)                                                                                           \
    ((c->player == PLAYER_ONE && c->pipIndex == PLAYER_ONE_BAR) ||                                                     \
     (c->player == PLAYER_TWO && c->pipIndex == PLAYER_TWO_BAR))

#define IS_PLAYERS_BAR(player, pipIndex)                                                                               \
    ((player == PLAYER_ONE && pipIndex == PLAYER_ONE_BAR) || (player == PLAYER_TWO && pipIndex == PLAYER_TWO_BAR))

struct Checker {
    Sint32 pipIndex;     // pip the checker is on
    Sint32 pipOffset;    // position of this checker on the pip
    Player player;       // player that owns the checker
    Uint32 numNeighbors; // number of checkers on the same pip
};

void createCheckerSprite(Checker* c);
