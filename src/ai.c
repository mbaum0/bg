/**
 * @file ai.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "ai.h"
#include <stdint.h>

int32_t evaluateBoard(GameBoard* gb, Color player, GameMoveSequence* moveSequence) {
  GameBoard tmp = { 0 };
  memcpy(&tmp, gb, sizeof(GameBoard));
  for (int32_t i = 0; i < moveSequence->numMoves; i++){
    moveChecker(&tmp, moveSequence->moves[i]);
  }
  int32_t score = getPlayerScore(&tmp, player);
  int32_t opponent = getPlayerScore(&tmp, OPPONENT_COLOR(player));
  return score - opponent;
}

// int32_t generateMoves(GameBoard* gb, Color player, GameMoveSequence* moveSequences) {
//   int32_t die1 = FIRST_DIE(gb)->value;
//   int32_t die2 = SECOND_DIE(gb)->value;
//   Checker* checker;
//   int32_t numSequences = 0;
//   GameMove gm;
// }

int32_t getPossibleMoves(GameBoard gb, Color player, GameMove* moves, int32_t amount) {
  Checker* checker;
  GameMove gm;
  int32_t numMoves = 0;
  for (int32_t i = 0; i < 2; i++) {
    if (player == LIGHT) {
      checker = &gb.lightCheckers[i];
    } else {
      checker = &gb.darkCheckers[i];
    }
    gm.srcPip = checker->pipIndex;
    gm.player = player;
    gm.amount = amount;

    if (isValidMove(&gb, gm)) {
      moves[numMoves] = gm;
      numMoves++;
    }
  }
  return numMoves;
}

int32_t generateMoves(GameBoard gb, Color player, GameMoveSequence* moveSequences) {
  int32_t numSequences = 0;
  GameBoard* gbptr = &gb;
  int32_t die1 = FIRST_DIE(gbptr)->value;
  int32_t die2 = SECOND_DIE(gbptr)->value;
  GameMove firstMoves[15] = {0};
  int32_t numFirstMoves = getPossibleMoves(gb, player, firstMoves, die1);
  saveCheckerState(gbptr);
  for (int32_t i = 0; i < numFirstMoves; i++) {
    moveChecker(gbptr, firstMoves[i]);
    GameMove secondMoves[15] = {0};
    int32_t numSecondMoves = getPossibleMoves(gb, player, secondMoves, die2);
    for (int32_t j = 0; j < numSecondMoves; j++) {
      moveSequences[numSequences].moves[0] = firstMoves[i];
      moveSequences[numSequences].moves[1] = secondMoves[j];
      moveSequences[numSequences].numMoves = 2;
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

  int32_t numOptions = generateMoves(tempGb, player, moveSequences);

  int32_t bestScore = 999;
  GameMoveSequence best = {0};
  for (int32_t i = 0; i < numOptions; i++) {
    GameMoveSequence gms = moveSequences[i];
    gms.resultScore = evaluateBoard(gb, player, &gms);
    if (gms.resultScore < bestScore) {
      bestScore = gms.resultScore;
      best = gms;
    }
  }
  return best;
}

void moveSequenceToString(GameMoveSequence gsm, char* dst) {
  if (gsm.numMoves == 1) {
    sprintf(dst, "pip: %d\nscore: %d\n", gsm.moves[0].srcPip, gsm.resultScore);
  } else if (gsm.numMoves == 2) {
    sprintf(dst, "pip: %d\npip: %d\nscore: %d\n", gsm.moves[0].srcPip, gsm.moves[1].srcPip, gsm.resultScore);
  } else if (gsm.numMoves == 3) {
    sprintf(dst, "pip: %d\npip: %d\npip: %d\nscore: %d\n", gsm.moves[0].srcPip, gsm.moves[1].srcPip,
            gsm.moves[2].srcPip, gsm.resultScore);
  } else if (gsm.numMoves == 4) {
    sprintf(dst, "pip: %d\npip: %d\npip: %d\npip: %d\nscore: %d\n", gsm.moves[0].srcPip, gsm.moves[1].srcPip,
            gsm.moves[2].srcPip, gsm.moves[3].srcPip, gsm.resultScore);
  }
}
