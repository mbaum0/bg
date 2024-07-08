/**
 * @file ai.h
 * @author Michael Baumgarten (you@domain.com)
 * @brief AI Player code
 */
#pragma once
#include "fsm.h"
#include "game.h"

/**
 * @brief Gets the best possible move sequence based on a single-turn score.
 */
void findBestMoveSequence(GameBoard* gb, Player player, GameMoveSequence* result);

/**
 * @brief Convert a move sequence to a string for logging.
 */
void moveSequenceToString(GameMoveSequence gsm, char* dst);
