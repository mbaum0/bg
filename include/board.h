/**
 * @file board.h
 * @author Michael Baumgarten
 * @brief Defines the GameBoard, Checkers, and other game related structs
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "checker.h"

typedef struct Dice Dice;
typedef struct GameBoard GameBoard;

#define LOC_HOME_LIGHT 0
#define LOC_HOME_DARK 25
#define LOC_BAR_LIGHT LOC_HOME_DARK
#define LOC_BAR_DARK LOC_HOME_LIGHT


struct Dice {
    int32_t die_0;
    int32_t die_1;
};

struct GameBoard {
    Checker checkers[30];
    Dice dice;
    int32_t clickedLocation;
    int32_t clickedSprite;
};

/**
 * @brief Return the number of checkers at a given location
 */
int32_t Board_getNumCheckersAtLocation(GameBoard* board, int32_t location);

/**
 * @brief Return the checker at the highest index at a given location
*/
Checker* Board_getNextCheckerAtLocation(GameBoard* board, int32_t location);

/**
 * @brief Return the owner of a given pip
 */
Player Board_getPipOwner(GameBoard* board, int32_t pip);

/**
 * @brief Returns true if a checker can move to a given location
 */
bool Board_canMoveChecker(GameBoard* board, Checker* checker, int32_t toLocation);

/**
 * @brief Move a checker to a given location
 */
void Board_moveChecker(GameBoard* board, Checker* checker, int32_t location);

/**
 * @brief Return the number of checkers a player has on the bar
 */
int32_t Board_getNumCheckersOnBar(GameBoard* board, Player player);

/**
 * @brief Roll the dice
 */
void Board_rollDice(GameBoard* board);

/**
 * @brief Moves a checker if possible. Returns true if the move was successful
 */
bool Board_moveIfPossible(GameBoard* board, int32_t fromLocation, int32_t amount);

/**
 * @brief Swap the dice
 */
void Board_swapDice(GameBoard* board);

/**
 * @brief Initialize the game board to a new game state. Must be freed with Board_free
 * 
 * @return GameBoard* 
 */
GameBoard* Board_init(void);

/**
 * @brief Free memory allocated by the game board
 */
void Board_free(GameBoard* board);
