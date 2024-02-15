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

    GameBoard gb0;
    deepCopy(&gb0, gb);
    bool swapDice = false;
    int32_t die1, die2;

    // do it once with the original dice, and once with them swapped
    for (int32_t p = 0; p < 2; p++) {
        if (swapDice) {
            die1 = SECOND_DIE(gb)->value;
            die2 = FIRST_DIE(gb)->value;
        } else {
            die1 = FIRST_DIE(gb)->value;
            die2 = SECOND_DIE(gb)->value;
        }
        GameMove firstMoves[15];

        int32_t firstMovesCount = getPossibleMoves(&gb0, player, die1, firstMoves);

        // do each possible first move, then test for second moves.
        for (int32_t i = 0; i < firstMovesCount; i++) {
            GameMove gm1 = firstMoves[i];
            GameBoard gb1;
            deepCopy(&gb1, &gb0);
            moveChecker(&gb1, gm1);
            GameMove secondMoves[15];
            int32_t secondMovesCount = getPossibleMoves(&gb1, player, die2, secondMoves);

            // test for second moves
            for (int32_t j = 0; j < secondMovesCount; j++) {
                GameMove gm2 = secondMoves[j];
                GameBoard gb2;
                deepCopy(&gb2, &gb1);
                moveChecker(&gb2, gm2);

                if (DOUBLES_ROLLED(gb)) {
                    GameMove thirdMoves[15];
                    int32_t thirdMovesCount = getPossibleMoves(&gb2, player, die1, thirdMoves);

                    // test for third moves
                    for (int32_t k = 0; k < thirdMovesCount; k++) {
                        GameMove gm3 = thirdMoves[k];
                        GameBoard gb3;
                        deepCopy(&gb3, &gb2);
                        moveChecker(&gb3, gm3);
                        GameMove fourthMoves[15];
                        int32_t fourthMovesCount = getPossibleMoves(&gb3, player, die2, fourthMoves);

                        // test for fourth moves
                        for (int32_t l = 0; l < fourthMovesCount; l++) {
                            GameMove gm4 = fourthMoves[l];
                            GameBoard gb4;
                            deepCopy(&gb4, &gb3);
                            moveChecker(&gb4, gm4);
                            // save off the 4-move option
                            moveSequences[numSequences].moves[0] = gm1;
                            moveSequences[numSequences].moves[1] = gm2;
                            moveSequences[numSequences].moves[2] = gm3;
                            moveSequences[numSequences].moves[3] = gm4;
                            moveSequences[numSequences].numMoves = 4;
                            moveSequences[numSequences].swapDice = swapDice;
                            moveSequences[numSequences].resultScore = evaluateBoard(&gb4, player);
                            numSequences++;
                            if (numSequences >= max) {
                                return numSequences;
                            }
                        }
                        // save off the 3-move option
                        moveSequences[numSequences].moves[0] = gm1;
                        moveSequences[numSequences].moves[1] = gm2;
                        moveSequences[numSequences].moves[2] = gm3;
                        moveSequences[numSequences].numMoves = 3;
                        moveSequences[numSequences].swapDice = swapDice;
                        moveSequences[numSequences].resultScore = evaluateBoard(&gb3, player);
                        numSequences++;
                        if (numSequences >= max) {
                            return numSequences;
                        }
                    }
                }

                // save off the 2-move option
                moveSequences[numSequences].moves[0] = gm1;
                moveSequences[numSequences].moves[1] = gm2;
                moveSequences[numSequences].numMoves = 2;
                moveSequences[numSequences].swapDice = swapDice;
                moveSequences[numSequences].resultScore = evaluateBoard(&gb2, player);
                numSequences++;
                if (numSequences >= max) {
                    return numSequences;
                }
            }

            // save off the single-move option
            moveSequences[numSequences].moves[0] = gm1;
            moveSequences[numSequences].numMoves = 1;
            moveSequences[numSequences].swapDice = swapDice;
            moveSequences[numSequences].resultScore = evaluateBoard(&gb1, player);
            numSequences++;
            if (numSequences >= max) {
                break;
            }
        }
        if (DOUBLES_ROLLED(gb)) {
            // we don't need to do a dice swap if there were doubles rolled
            break;
        }
        swapDice = true;
    }
    return numSequences;
}

GameMoveSequence findBestMoveSequence(GameBoard* gb, Color player) {
    GameMoveSequence moveSequences[MAX_SEQUENCES] = {0};

    int32_t numOptions = generateMovesSequences(gb, player, moveSequences, MAX_SEQUENCES);

    int32_t bestScore = 999;
    GameMoveSequence best = {0};
    for (int32_t i = 0; i < numOptions; i++) {
        GameMoveSequence gms = moveSequences[i];
        if (gms.resultScore < bestScore) {
            bestScore = gms.resultScore;
            best = gms;
        }
    }
    int32_t currentScore = evaluateBoard(gb, player);
    log_debug("AI found %d move options. Best option will alter game by %d points", numOptions,
              (currentScore - best.resultScore));
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
