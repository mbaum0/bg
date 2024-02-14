/**
 * @file ai.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "ai.h"
#include <stdint.h>

int32_t evaluateBoard(GameBoard* gb, Color player) {
  int32_t score = getPlayerScore(gb, player);
  int32_t opponent = getPlayerScore(gb, OPPONENT_COLOR(player));
  return score - opponent;
}

// int32_t generateMoves(GameBoard* gb, Color player, GameMoveSequence* moveSequences) {
//   int32_t die1 = FIRST_DIE(gb)->value;
//   int32_t die2 = SECOND_DIE(gb)->value;
//   Checker* checker;
//   int32_t numSequences = 0;
//   GameMove gm;
// }

int32_t generateMoves(GameBoard gb, Color player, GameMoveSequence* moveSequences, int32_t numSequences, int32_t die1,
                      int32_t die2, int32_t depth) {
  if (depth == 0) {
    moveSequences[numSequences].resultScore = getPlayerScore(&gb, player);
    numSequences++;
    return numSequences;
  }

  Checker* checker;
  GameMove gm;
  for (int32_t i = 0; i < 15; i++) {
    if (player == LIGHT) {
      checker = &gb.lightCheckers[i];
    } else {
      checker = &gb.darkCheckers[i];
    }
    gm.c = checker;
    gm.amount = (depth == 2) ? die1 : die2;
    if (isValidMove(&gb, gm)) {
      moveChecker(&gb, gm);
      moveSequences[numSequences].moves[moveSequences[numSequences].numMoves] = gm;
      moveSequences[numSequences].numMoves++;
      numSequences += generateMoves(gb, player, moveSequences, numSequences, die1, die2, depth - 1);
    } else {
      numSequences++;
    }
  }
  return numSequences;
}

GameMoveSequence findBestMoveSequence(GameBoard* gb, Color player) {
  // make a temporary board for evaluating moves
  GameBoard tempGb;
  tempGb.die1 = gb->die1;
  tempGb.die2 = gb->die2;
  memcpy(tempGb.darkCheckers, gb->darkCheckers, sizeof(gb->darkCheckers));
  memcpy(tempGb.lightCheckers, gb->lightCheckers, sizeof(gb->lightCheckers));
  saveCheckerState(&tempGb); // copy the checker state so we can restore it each evaluation
  GameMoveSequence moveSequences[400] = {0};

  int32_t die1 = FIRST_DIE(gb)->value;
  int32_t die2 = SECOND_DIE(gb)->value;
  int32_t numOptions = generateMoves(tempGb, player, moveSequences, 0, die1, die2, 2);

  int32_t bestScore = 999;
  GameMoveSequence best = {0};
  for (int32_t i = 0; i < numOptions; i++) {
    GameMoveSequence gms = moveSequences[i];
    if (gms.resultScore < bestScore) {
      bestScore = gms.resultScore;
      best = gms;
    }
  }
  return best;
}

void moveSequenceToString(GameMoveSequence gsm, char* dst) {
  if (gsm.numMoves == 1) {
    sprintf(dst, "pip: %d\nscore: %d\n", gsm.moves[0].c->pipIndex, gsm.resultScore);
  } else if (gsm.numMoves == 2) {
    sprintf(dst, "pip: %d\npip: %d\nscore: %d\n", gsm.moves[0].c->pipIndex, gsm.moves[1].c->pipIndex, gsm.resultScore);
  } else if (gsm.numMoves == 3) {
    sprintf(dst, "pip: %d\npip: %d\npip: %d\nscore: %d\n", gsm.moves[0].c->pipIndex, gsm.moves[1].c->pipIndex,
            gsm.moves[2].c->pipIndex, gsm.resultScore);
  } else if (gsm.numMoves == 4) {
    sprintf(dst, "pip: %d\npip: %d\npip: %d\npip: %d\nscore: %d\n", gsm.moves[0].c->pipIndex, gsm.moves[1].c->pipIndex,
            gsm.moves[2].c->pipIndex, gsm.moves[3].c->pipIndex, gsm.resultScore);
  }
}
