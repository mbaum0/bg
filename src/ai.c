/**
 * @file ai.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "ai.h"

Sint32 evaluateBoard(GameBoard* gb, Color player) {
    Sint32 score = getPlayerScore(gb, player);
    Sint32 opponent = getPlayerScore(gb, OPPONENT_COLOR(player));
    return score - opponent;
}

Sint32 getPossibleMoves(GameBoard* gb, Color player, Sint32 dieValue, GameMove* moves) {
    GameMove gm;
    Sint32 numMoves = 0;
    gm.player = player;
    gm.amount = dieValue;
    for (Sint32 i = 0; i <= DARK_BAR; i++) {
        gm.srcPip = i;
        if (isValidMove(gb, gm)) {
            moves[numMoves] = gm;
            numMoves++;
        }
    }
    return numMoves;
}

Sint32 generateMovesSequences(GameBoard* gb, Color player, GameMoveSequence* moveSequences, Sint32 max,
                              Sint32* maxMoves) {
    *maxMoves = 0;
    Sint32 numSequences = 0;

    GameBoard gb0;
    deepCopy(&gb0, gb);
    bool swapDice = false;
    Sint32 die1, die2;

    // do it once with the original dice, and once with them swapped
    for (Sint32 p = 0; p < 2; p++) {
        if (swapDice) {
            die1 = SECOND_DIE(gb)->value;
            die2 = FIRST_DIE(gb)->value;
        } else {
            die1 = FIRST_DIE(gb)->value;
            die2 = SECOND_DIE(gb)->value;
        }
        GameMove firstMoves[15];

        Sint32 firstMovesCount = getPossibleMoves(&gb0, player, die1, firstMoves);

        // do each possible first move, then test for second moves.
        for (Sint32 i = 0; i < firstMovesCount; i++) {
            GameMove gm1 = firstMoves[i];
            GameBoard gb1;
            deepCopy(&gb1, &gb0);
            moveChecker(&gb1, gm1);
            GameMove secondMoves[15];
            Sint32 secondMovesCount = getPossibleMoves(&gb1, player, die2, secondMoves);

            // test for second moves
            for (Sint32 j = 0; j < secondMovesCount; j++) {
                GameMove gm2 = secondMoves[j];
                GameBoard gb2;
                deepCopy(&gb2, &gb1);
                moveChecker(&gb2, gm2);

                if (DOUBLES_ROLLED(gb)) {
                    GameMove thirdMoves[15];
                    Sint32 thirdMovesCount = getPossibleMoves(&gb2, player, die1, thirdMoves);

                    // test for third moves
                    for (Sint32 k = 0; k < thirdMovesCount; k++) {
                        GameMove gm3 = thirdMoves[k];
                        GameBoard gb3;
                        deepCopy(&gb3, &gb2);
                        moveChecker(&gb3, gm3);
                        GameMove fourthMoves[15];
                        Sint32 fourthMovesCount = getPossibleMoves(&gb3, player, die2, fourthMoves);

                        // test for fourth moves
                        for (Sint32 l = 0; l < fourthMovesCount; l++) {
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
                            if (4 > *maxMoves) {
                                *maxMoves = 4;
                            }
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
                        if (3 > *maxMoves) {
                            *maxMoves = 3;
                        }
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
                if (2 > *maxMoves) {
                    *maxMoves = 2;
                }
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
            if (1 > *maxMoves) {
                *maxMoves = 1;
            }
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

void findBestMoveSequence(GameBoard* gb, Color player, GameMoveSequence* result) {

    Sint32 maxMoves = 0;
    Sint32 numOptions = generateMovesSequences(gb, player, gb->aiMoves, MAX_AI_SEQUENCES, &maxMoves);

    Sint32 bestScore = 999;
    GameMoveSequence best = {0};
    for (Sint32 i = 0; i < numOptions; i++) {
        GameMoveSequence gms = gb->aiMoves[i];
        if (gms.numMoves < maxMoves) {
            // backgammon rules require player to use maximum number of dice possible;
            continue;
        }
        if (gms.resultScore < bestScore) {
            bestScore = gms.resultScore;
            best = gms;
        }
    }
    Sint32 currentScore = evaluateBoard(gb, player);
    Sint32 scoreDelta = (numOptions == 0) ? 0 : (currentScore - best.resultScore);
    log_debug("AI found %d move options. Best option will alter game by %d points", numOptions, scoreDelta);
    *result = best;
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
