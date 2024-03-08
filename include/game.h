/**
 * @file game.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#pragma once
#include "buttons.h"
#include "checker.h"
#include "dialog.h"
#include "dice.h"
#include "pip.h"
#include <stdbool.h>

#define FIRST_DIE(gb) ((gb->die1.index == 0) ? &gb->die1 : &gb->die2)
#define SECOND_DIE(gb) ((gb->die2.index == 1) ? &gb->die2 : &gb->die1)
#define DOUBLES_ROLLED(gb) ((gb->die1.value == gb->die2.value))
#define NUM_MOVES_MADE(gb) ((gb->die1.uses + gb->die2.uses))
#define PLAYER_CHECKERS(gb, color) ((color == LIGHT) ? gb->lightCheckers : gb->darkCheckers)
#define SET_PIP_COLOR(gb, pipIndex, pcolor)                                                                            \
    if ((pipIndex) > 0 && (pipIndex) < 25) {                                                                           \
        (gb)->pips[(pipIndex - 1)].color = (pcolor);                                                                   \
    }

#define MAX_AI_SEQUENCES (15 * 15)

typedef struct GameMove GameMove;
struct GameMove {
    Color player;
    Sint32 srcPip;
    Sint32 amount;
};

typedef struct GameMoveSequence GameMoveSequence;
/**
 * GameMoveSequence contains an array of GameMoves which
 * represents a turn. If swapDice is true, the dice should
 * be swapped at the beginning of the sequence to make it valid.
 */
struct GameMoveSequence {
    GameMove moves[4];
    Sint32 numMoves;
    Sint32 resultScore;
    bool swapDice;
};

typedef struct GameBoard GameBoard;

struct GameBoard {
    Checker lightCheckers[15];
    Checker darkCheckers[15];
    Checker lightCheckersSave[15];
    Checker darkCheckersSave[15];
    Pip pips[24];
    GameDie die1;
    GameDie die2;
    GameButton confirm;
    GameButton undo;
    GameButton roll;
    GameButton dub;
    GameButton nomoves;
    Dialog dialog;
    Color activePlayer;
    Color aiPlayer;
    GameMoveSequence* aiMoves;
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
Checker* getTopCheckerOnPip(GameBoard* gb, Sint32 pipIndex);

/**
 * @brief Returns true if all of the player's checkers are within
 * their inner table.
 */
bool playerHasClosedBoard(GameBoard* gb, Color player);

/**
 * @brief Get the number of checkers on a given pip.
 */
Sint32 getNumCheckersOnPip(GameBoard* gb, Sint32 pipIndex);

/**
 * @brief Returns the index of the destination pip if a checker
 * if moved from the source pip the given amount.
 */
Sint32 getNextPip(Sint32 pipIndex, Color player, Sint32 amount);

/**
 * @brief Returns the value of the next available die. Returns -1
 * if there are no more moves left.
 */
Sint32 getNextDieValue(GameBoard* gb);

/**
 * @brief Swaps the order of the dice if neither die is used.
 */
void swapDiceIfAllowed(GameBoard* gb);

/**
 * @brief Increments the move counter and returns the number
 * of moves the player has left
 */
Sint32 incrementMoveCount(GameBoard* gb);

/**
 * @brief Inits the board for player move state. This includes
 * dice positions, animations, button visibility, etc.
 */
void initBoardForPlayerMove(GameBoard* gb);

/**
 * @brief Inits the board for the roll for first state.
 */
void initBoardForRollForFirst(GameBoard* gb);

/**
 * @brief Updates the display of the board during the player move state.
 */
void updateBoardForPlayerMove(GameBoard* gb);

/**
 * @brief Inits the board for dice roll state. This includes
 * dice positions, animations, button visibility, etc.
 */
void initBoardForDiceRoll(GameBoard* gb);

/**
 * @brief Inits the board for the confirm state. This includes
 * the undo/confirm button visibility.
 */
void initBoardForConfirm(GameBoard* gb);

/**
 * @brief Inits the board for the initial state of the game. This includes
 * the undo/confirm/roll button visibility.
 */
void initBoardForInit(GameBoard* gb);

/**
 * @brief Returns true if the given pip is occupied by a single
 * opponent checker. Opponent is the non-active player.
 */
bool isPipOpponentBlot(GameBoard* gb, Sint32 pipIndex);

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
Sint32 getPlayerScore(GameBoard* gb, Color player);

/**
 * @brief Returns Light/Dark if there is a winner, else None.
 */
Color getMatchWinner(GameBoard* gb);

/**
 * @brief Creates a deep copy of the src GameBoard and places
 * it into dst.
 */
void deepCopy(GameBoard* dst, GameBoard* src);

/**
 * @brief Returns true if either die has been used;
 */
bool haveDiceBeenUsed(GameBoard* gb);

/**
 * @brief Reset the gameboard for the next match
 */
void gameboard_reset(GameBoard* gb);
