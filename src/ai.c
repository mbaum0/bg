/**
 * @file ai.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "ai.h"
#include "wildbg.h"

Sint32 getWeightedPlayerScore(GameBoard* gb, Color player) {
    Sint32 score = 0;
    Checker* checkers;
    if (player == LIGHT) {
        checkers = gb->lightCheckers;
    } else {
        checkers = gb->darkCheckers;
    }
    for (Sint32 i = 0; i < 15; i++) {
        Sint32 pipIndex = checkers[i].pipIndex;
        if (pipIndex == LIGHT_HOME || pipIndex == DARK_HOME) {
            // add nothing here because checker is home
            continue;
        }

        if (pipIndex == LIGHT_BAR || pipIndex == DARK_BAR) {
            score += 25;
        }

        if (player == DARK) {
            score += (DARK_HOME - pipIndex);
        } else {
            score += pipIndex;
        }

        if (checkers[i].numNeighbors == 0) {
            score += 100;
        }
    }
    return score;
}

Sint32 evaluateBoard(GameBoard* gb, Color player) {
    Sint32 score = getWeightedPlayerScore(gb, player);
    Sint32 opponent = getWeightedPlayerScore(gb, OPPONENT_COLOR(player));
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

    // Sint32 maxMoves = 0;
    // Sint32 numOptions = generateMovesSequences(gb, player, gb->aiMoves, MAX_AI_SEQUENCES, &maxMoves);

    // Sint32 bestScore = 999;
    // GameMoveSequence best = {0};
    // for (Sint32 i = 0; i < numOptions; i++) {
    //     GameMoveSequence gms = gb->aiMoves[i];
    //     if (gms.numMoves < maxMoves) {
    //         // backgammon rules require player to use maximum number of dice possible;
    //         continue;
    //     }
    //     if (gms.resultScore < bestScore) {
    //         bestScore = gms.resultScore;
    //         best = gms;
    //     }
    // }
    // Sint32 currentScore = evaluateBoard(gb, player);
    // Sint32 scoreDelta = (numOptions == 0) ? 0 : (currentScore - best.resultScore);
    // log_debug("AI found %d move options. Best option will alter game by %d points", numOptions, scoreDelta);
    // *result = best;

    GameMoveSequence best = {0};

    Wildbg* wildbg = wildbg_new();
    if (wildbg == NULL) {
        log_error("Failed to load neural nets");
        return;
    }
    // ai is dark
    int pips[26] = {0};
    for (Sint32 i = 0; i < 15; i++) {
        Checker* c = &gb->lightCheckers[i];
        if (c->pipIndex == LIGHT_BAR) {
            pips[0] -= 1;
        } else if (c->pipIndex != LIGHT_HOME) {
            pips[25 - c->pipIndex] -= 1;
        }

        c = &gb->darkCheckers[i];
        if (c->pipIndex == DARK_BAR) {
            pips[25] += 1;
        } else if (c->pipIndex != DARK_HOME) {
            pips[25 - c->pipIndex] += 1;
        }
    }

    BgConfig config = {.x_away = 1, .o_away = 1};
    CMove move = best_move(wildbg, &pips, FIRST_DIE(gb)->value, SECOND_DIE(gb)->value, &config);
    log_debug("WildBG would move from %d to %d and from %d to %d.", 25 - move.detail1.from, 25 - move.detail1.to,
              25 - move.detail2.from, 25 - move.detail2.to);
    wildbg_free(wildbg);

    if (move.detail1.from != -1) {
        int dist = abs(move.detail1.from - move.detail1.to);
        if (dist != FIRST_DIE(gb)->value) {
            best.swapDice = true;
        }
        if (move.detail1.from == 25) {
            move.detail1.from = 25 - DARK_BAR;
        }
        GameMove gm1 = {.player = player, .srcPip = 25 - move.detail1.from, .amount = dist};
        best.moves[0] = gm1;
        best.numMoves = 1;
    }

    if (move.detail2.from != -1) {
        int dist = abs(move.detail2.from - move.detail2.to);
        if (move.detail2.from == 25) {
            move.detail2.from = 25 - DARK_BAR;
        }
        GameMove gm2 = {.player = player, .srcPip = 25 - move.detail2.from, .amount = dist};
        best.moves[1] = gm2;
        best.numMoves = 2;
    }

    if (move.detail3.from != -1) {
        int dist = abs(move.detail3.from - move.detail3.to);
        if (move.detail3.from == 25) {
            move.detail3.from = 25 - DARK_BAR;
        }
        GameMove gm3 = {.player = player, .srcPip = 25 - move.detail3.from, .amount = dist};
        best.moves[2] = gm3;
        best.numMoves = 3;
    }
    if (move.detail4.from != -1) {
        int dist = abs(move.detail4.from - move.detail4.to);
        if (move.detail4.from == 25) {
            move.detail4.from = 25 - DARK_BAR;
        }
        GameMove gm4 = {.player = player, .srcPip = 25 - move.detail4.from, .amount = dist};
        best.moves[3] = gm4;
        best.numMoves = 4;
    }
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
