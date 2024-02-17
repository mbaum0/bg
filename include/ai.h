/**
 * @file ai.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief AI Player code
 */
#pragma once
#include "fsm.h"
#include "game.h"

#define MAX_SEQUENCES (15 * 15 * 15 * 15)

/**
 * @brief Evaluate the score of the gameboard in terms
 * of the given player
 * ie (PLAYER - OPPONENT)
 * Greater values are better for the given player.
 */
Sint32 evaluateBoard(GameBoard* gb, Color player);

/**
 * @brief Generates all possible move sequences for the player from
 * a given state and stores them in the provided array. Returns the number
 * of generated sequences.
 */
Sint32 generateMovesSequences(GameBoard* gb, Color player, GameMoveSequence* moveSequences, Sint32 max);

/**
 * @brief Generates a list of possible next moves. Returns the number of moves found. There
 * can be at most 15 moves as there are 15 checkers per player.
 */
Sint32 getPossibleMoves(GameBoard* gb, Color player, Sint32 dieValue, GameMove* moves);

/**
 * @brief Gets the best possible move sequence based on a single-turn score.
 */
void findBestMoveSequence(GameBoard* gb, Color player, GameMoveSequence* result);

/**
 * @brief Convert a move sequence to a string for logging.
 */
void moveSequenceToString(GameMoveSequence gsm, char* dst);
