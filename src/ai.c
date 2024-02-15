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

int32_t getPossibleMoves(GameBoard* gb, Color player, int32_t dieValue, GameMove* moves) {
    GameMove gm;
    int32_t numMoves = 0;

    Checker* checkers = PLAYER_CHECKERS(gb, player);
    gm.player = player;
    gm.amount = dieValue;
    for (int32_t i = 0; i < 15; i++) {
        gm.srcPip = checkers[i].pipIndex;
        if (isValidMove(gb, gm)) {
            moves[numMoves] = gm;
            numMoves++;
        }
    }
    return numMoves;
}

int32_t generateMovesSequences(GameBoard* gb, Color player, GameMoveSequence* moveSequences, int32_t max) {
    int32_t numSequences = 0;
    int32_t die1 = FIRST_DIE(gb)->value;
    int32_t die2 = SECOND_DIE(gb)->value;

    GameMove firstMoves[15];

    int32_t firstMovesCount = getPossibleMoves(gb, player, die1, firstMoves);

    // do each possible first move, then test for second moves.
    for (int32_t i = 0; i < firstMovesCount; i++) {
        GameMove gm1 = firstMoves[i];
        GameBoard gb1;
        deepCopy(&gb1, gb);
        moveChecker(&gb1, gm1);
        GameMove secondMoves[15];
        int32_t secondMovesCount = getPossibleMoves(&gb1, player, die2, secondMoves);

        // test for second moves
        for (int32_t j = 0; j < secondMovesCount; j++) {
            GameMove gm2 = secondMoves[j];
            GameBoard gb2;
            deepCopy(&gb2, &gb1);
            moveChecker(&gb2, gm2);
            moveSequences[numSequences].moves[0] = gm1;
            moveSequences[numSequences].moves[1] = gm2;
            moveSequences[numSequences].numMoves = 2;
            moveSequences[numSequences].resultScore = evaluateBoard(&gb2, player);
            numSequences++;
            if (numSequences >= max) {
                break;
            }
        }

        // save off the single-move option
        moveSequences[numSequences].moves[0] = gm1;
        moveSequences[numSequences].numMoves = 1;
        moveSequences[numSequences].resultScore = evaluateBoard(&gb1, player);
        numSequences++;
        if (numSequences >= max) {
            break;
        }
    }
    return numSequences;
}

GameMoveSequence findBestMoveSequence(GameBoard* gb, Color player) {
    GameMoveSequence moveSequences[400] = {0};

    int32_t numOptions = generateMovesSequences(gb, player, moveSequences, 400);

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
