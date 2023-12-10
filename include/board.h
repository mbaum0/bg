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
#include "dice.h"

typedef struct GameBoard GameBoard;

#define LOC_HOME_LIGHT 0
#define LOC_HOME_DARK 25
#define LOC_BAR_LIGHT LOC_HOME_DARK
#define LOC_BAR_DARK LOC_HOME_LIGHT

typedef void state_fn(GameBoard* board);
void state_init(GameBoard* board);
void state_diceRoll(GameBoard* board);
void state_checkAvailableMoves(GameBoard* board);
void state_getMove(GameBoard* board);

struct GameBoard {
    Checker checkers[30];
    Die die0;
    Die die1;
    int32_t clickedLocation;
    int32_t clickedSprite;
    int32_t playerMoveCount;
    char stateName[20];
    bool diceRolled;
    Player activePlayer;
    state_fn* state;
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
 * @brief Returns true if a checker can move a given amount
 */
bool Board_canMoveChecker(GameBoard* board, Checker* checker, int32_t amount);

/**
 * @brief Move a checker to a given location
 */
void Board_moveChecker(GameBoard* board, Checker* checker, int32_t location);

/**
 * @brief Return the number of checkers a player has on the bar
 */
int32_t Board_getNumCheckersOnBar(GameBoard* board, Player player);

/**
 * @brief Moves a checker if possible. Returns true if the move was successful
 */
bool Board_moveIfPossible(GameBoard* board, int32_t fromLocation, int32_t amount);

/**
 * @brief Returns the number of possible moves for the active player given the current dice roll
 */
int32_t Board_getPossibleMoves(GameBoard* board);

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


/**
 * @brief Export the game board to a file
 */
void Board_export(GameBoard* board, char* filename);

/**
 * @brief Import a game board from a file
 */
void Board_import(GameBoard* board, char* filename);
