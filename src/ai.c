/**
 * @file ai.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "ai.h"
#include "wildbg.h"

void convertPipsToWildBgFormat(GameBoard* gb, Player player, int (*pips)[26]) {
    for (Sint32 i = 0; i < 15; i++) {
        Checker* c = &gb->p1Checkers[i];
        if (IS_CHECKER_BARRED(c)) {
            (*pips)[PLAYER_ONE_BAR] += 1;
        } else if (c->pipIndex != PLAYER_ONE_HOME) {
            (*pips)[c->pipIndex] += 1;
        }

        c = &gb->p2Checkers[i];
        if (IS_CHECKER_BARRED(c)) {
            (*pips)[PLAYER_TWO_BAR] -= 1;
        } else if (c->pipIndex != PLAYER_TWO_HOME) {
            (*pips)[c->pipIndex] -= 1;
        }
    }

    if (player == PLAYER_TWO) {
        // swap pip index eg. 0->25, 1->24, 2->23, etc.
        for (int i = 0; i < 13; i++) {
            int temp = (*pips)[i];
            (*pips)[i] = (*pips)[25 - i] * -1;
            (*pips)[25 - i] = temp * -1;
        }
    }
}

bool convertMoveDetailToGameMove(CMoveDetail* moveDetail, GameMove* gm, Player player) {
    if (moveDetail->from == -1) {
        return false;
    }

    if (player == PLAYER_ONE) {
        gm->srcPip = moveDetail->from;
        gm->amount = abs(moveDetail->from - moveDetail->to);
        gm->player = player;
    } else {
        gm->srcPip = 25 - moveDetail->from;
        gm->amount = abs(moveDetail->from - moveDetail->to);
        gm->player = player;
    }
    return true;
}

void findBestMoveSequence(GameBoard* gb, Player player, GameMoveSequence* result) {

    GameMoveSequence best = {0};

    Wildbg* wildbg = wildbg_new();
    if (wildbg == NULL) {
        log_error("Failed to load neural nets");
        return;
    }

    int pips[26] = {0};
    convertPipsToWildBgFormat(gb, player, &pips);

    BgConfig config = {.x_away = 1, .o_away = 1};
    CMove move = best_move(wildbg, &pips, FIRST_DIE(gb)->value, SECOND_DIE(gb)->value, &config);
    log_debug("WildBG would move from %d to %d and from %d to %d.", 25 - move.detail1.from, 25 - move.detail1.to,
              25 - move.detail2.from, 25 - move.detail2.to);
    wildbg_free(wildbg);

    GameMove gm = {0};
    if (convertMoveDetailToGameMove(&move.detail1, &gm, player)) {
        best.moves[best.numMoves] = gm;
        if (gm.amount != FIRST_DIE(gb)->value) {
            best.swapDice = true;
        }
        best.numMoves++;
    }
    if (convertMoveDetailToGameMove(&move.detail2, &gm, player)) {
        best.moves[best.numMoves] = gm;
        best.numMoves++;
    }
    if (convertMoveDetailToGameMove(&move.detail3, &gm, player)) {
        best.moves[best.numMoves] = gm;
        best.numMoves++;
    }
    if (convertMoveDetailToGameMove(&move.detail4, &gm, player)) {
        best.moves[best.numMoves] = gm;
        best.numMoves++;
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
