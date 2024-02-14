/**
 * @file game.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#pragma once
#include "fsm.h"
#include <stdbool.h>

#define FIRST_DIE(gb) ((gb->die1.index == 0) ? &gb->die1 : &gb->die2)
#define SECOND_DIE(gb) ((gb->die2.index == 1) ? &gb->die2 : &gb->die1)
#define DOUBLES_ROLLED(gb) ((gb->die1.value == gb->die2.value))
#define PLAYER_CHECKERS(gb, color) ((color == LIGHT) ? gb->lightCheckers : gb->darkCheckers)

typedef struct GameMove GameMove;
struct GameMove {
  Checker* c;
  int32_t amount;
};

typedef struct GameMoveSequence GameMoveSequence;
struct GameMoveSequence {
  GameMove moves[4];
  int32_t numMoves;
  int32_t resultScore;
};

/**
 * @brief Initialize the gameboard to a new game state.
 */
void gameboard_init(void);

/**
 * @brief Move the given checker the provided amount. This function
 * has no checks for validity.
 */
void moveChecker(GameBoard* gb, GameMove gm);

/**
 * @brief Returns true if the provided checker is able to move
 * the provided amount.
 */
bool isValidMove(GameBoard* gb, GameMove gm);

/**
 * @brief Get the top checker on a given pip.
 */
Checker* getTopCheckerOnPip(GameBoard* gb, int32_t pipIndex);

/**
 * @brief Returns true if all of the player's checkers are within
 * their inner table.
 */
bool playerHasClosedBoard(GameBoard* gb, Color player);

/**
 * @brief Get the number of checkers on a given pip.
 */
int32_t getNumCheckersOnPip(GameBoard* gb, int32_t pipIndex);

/**
 * @brief Returns the index of the destination pip if the given
 * checker is move the provided amount.
 */
int32_t getNextPip(Checker* c, int32_t amount);

/**
 * @brief Returns the value of the next available die. Returns -1
 * if there are no more moves left.
 */
int32_t getNextDieValue(GameBoard* gb);

/**
 * @brief Swaps the order of the dice if neither die is used.
 */
void swapDiceIfAllowed(GameBoard* gb);

/**
 * @brief Increments the move counter and returns the number
 * of moves the player has left
 */
int32_t incrementMoveCount(GameBoard* gb);

/**
 * @brief Updates the board for player move state. This includes
 * dice positions, animations, button visibility, etc.
 */
void updateBoardForPlayerMove(GameBoard* gb);

/**
 * @brief Updates the board for dice roll state. This includes
 * dice positions, animations, button visibility, etc.
 */
void updateBoardForDiceRoll(GameBoard* gb);

/**
 * @brief Updates the board for the confirm state. This includes
 * the undo/confirm button visibility.
 */
void updateBoardForConfirm(GameBoard* gb);

/**
 * @brief Returns true if the given pip is occupied by a single
 * opponent checker. Opponent is the non-active player.
 */
bool isPipOpponentBlot(GameBoard* gb, int32_t pipIndex);

/**
 * @brief Move the provided checker to its player's bar
 */
void barChecker(GameBoard* gb, Checker* c);

/**
 * @brief Roll both dice
 */
void rollDice(GameBoard* gb);

/**
 * @brief Returns true if the active player has barred checkers
 */
bool playerHasCheckersOnBar(GameBoard* gb);

/**
 * @brief Copies the state of the checkers into
 * the save buffer
 */
void saveCheckerState(GameBoard* gb);

/**
 * @brief Restores the contents of the save buffer into
 * the gameboard state
 */
void loadCheckerState(GameBoard* gb);

/**
 * @brief Returns true if the active player has any valid moves
 */
bool playerHasMoves(GameBoard* gb);

/**
 * @brief Return the current score of the player. This is a
 * weighted value based on the distance of all of their checkers
 * to their home. Less is better
 */
int32_t getPlayerScore(GameBoard* gb, Color player);
