/**
 * @file ai.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief AI Player code
 */
#pragma once
#include "fsm.h"
#include "game.h"

/**
 * @brief Evaluate the score of the gameboard in terms
 * of the given player
 * ie (PLAYER - OPPONENT)
 * Greater values are better for the given player.
 */
int32_t evaluateBoard(GameBoard* gb, Color player, GameMoveSequence* moveSequence);

/**
 * @brief Generates all possible move sequences for the player from
 * a given state and stores them in the provided array. Returns the number
 * of generated sequences.
 */
int32_t generateMoves(GameBoard gb, Color player, GameMoveSequence* moveSequences);

/**
 * @brief Generates a list of possible next moves. Returns the number of moves found.
 */
int32_t getPossibleMoves(GameBoard gb, Color player, GameMove* moves, int32_t amount);

/**
 * @brief Gets the best possible move sequence based on a single-turn score.
 */
GameMoveSequence findBestMoveSequence(GameBoard* gb, Color player);

/**
 * @brief Convert a move sequence to a string for logging.
 */
void moveSequenceToString(GameMoveSequence gsm, char* dst);
