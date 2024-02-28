/**
 * @file player_move.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "ai.h"
#include "fsm.h"
#include "game.h"
#include <stdio.h>

/**
 * This is a hacky solution to combine the gameboard
 * and a move so that the AI can pass both of them
 * to the timer function.
 */
typedef struct GameBoardMove GameBoardMove;

struct GameBoardMove {
    GameBoard* gb;
    GameMove move;
};

Uint32 timerEndPlayerTurnIfNoMoves(Uint32 interval, void* ctx) {
    (void)interval;
    GameBoard* gb = (GameBoard*)ctx;
    GameDie* die1 = FIRST_DIE(gb);
    bool hasBothDiceLeft = (die1->uses == 0);

    if (gb->activePlayer == gb->aiPlayer){
        // this function may get called after a player has finished their turn and incidentially
        // run while the ai has just begun their turn. This timer does not apply to the AI, as
        // their code handles the zero-move scenario.
        return 0;
    }

    if (!playerHasMoves(gb)) {
        log_debug("Active player has no moves available");
        if (hasBothDiceLeft) {
            gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
            fsm_transition(WAIT_FOR_ROLL_STATE);
        } else {
            fsm_transition(MOVE_CONFIRM_STATE);
        }
    }
    return 0;
}

Uint32 timerPipClickFade(Uint32 interval, void* ctx) {
    Pip* pip = (Pip*)ctx;
    if (pip->alpha == 0) {
        // start from full opacity
        pip->alpha = 255;
        return interval;
    }

    Sint32 newAlpha = pip->alpha - 10;
    if (newAlpha <= 0) {
        pip->alpha = 0;
        return 0;
    }
    pip->alpha = newAlpha;
    return interval;
}

void doAiMove(GameBoard* gb, GameMove gm) {
    Checker* c = getTopCheckerOnPip(gb, gm.srcPip);
    if (c == NULL) {
        log_error("AI picked an empty pip.");
        return;
    }

    Sint32 dieValue = getNextDieValue(gb);

    if (dieValue != gm.amount) {
        log_error("AI picked invalid die value.");
        return;
    }

    if (isValidMove(gb, gm)) {
        moveChecker(gb, gm);
        incrementMoveCount(gb);
    } else {
        log_error("AI picked an invalid move.");
    }
}

Uint32 timerDoAiMove(Uint32 interval, void* ctx) {
    (void)interval;
    GameBoardMove* gbm = (GameBoardMove*)ctx;
    doAiMove(gbm->gb, gbm->move);
    SDL_free(gbm);
    return 0;
}

Uint32 timerEndAiMove(Uint32 interval, void* ctx) {
    (void)interval;
    GameBoard* gb = (GameBoard*)ctx;
    gb->nomoves.visible = false; // hide this in case it was made visible
    gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
    fsm_transition(WAIT_FOR_ROLL_STATE);
    return 0;
}

Uint32 timerSetNoMovesVisible(Uint32 interval, void* ctx){
    (void)interval;
    GameBoard* gb = (GameBoard*)ctx;
    gb->nomoves.visible = true;
    return 0;
}

Uint32 timeDoAiSwapDice(Uint32 interval, void* ctx) {
    (void)interval;
    GameBoard* gb = (GameBoard*)ctx;
    swapDiceIfAllowed(gb);
    return 0;
}

void doPlayerMove(GameBoard* gb, Uint32 pipIndex) {
    Sint32 movesLeft = 0;
    Checker* c = getTopCheckerOnPip(gb, pipIndex);
    bool success = true;
    if (c == NULL) {
        success = false;
        goto final;
    }

    Sint32 dieValue = getNextDieValue(gb);
    GameMove gm = {c->color, c->pipIndex, dieValue};

    if (isValidMove(gb, gm)) {
        moveChecker(gb, gm);
        movesLeft = incrementMoveCount(gb);
        success = true;
    } else {
        success = false;
        goto final;
    }

    if (movesLeft == 0) {
        fsm_transition(MOVE_CONFIRM_STATE);
    } else {
        SDL_AddTimer(1000, timerEndPlayerTurnIfNoMoves, gb);
    }

final:
    if (pipIndex > 0 && pipIndex < 25) {
        Pip* pip = (Pip*)&gb->pips[pipIndex - 1];
        if (success) {
            pip->color = PIP_BLUE;
        } else {
            pip->color = PIP_RED;
        }
        pip->alpha = 255;
    }
}

void doDiceSwap(GameBoard* gb) {
    swapDiceIfAllowed(gb);
}

void player_move_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    updateBoardForPlayerMove(gb);
    while (fsm_dequeue_event(&event)) {
        if (event.etype == PIP_CLICKED_EVENT) {
            // player can't pick for ai
            if (gb->activePlayer != gb->aiPlayer) {
                doPlayerMove(gb, event.code);
            }
        }

        if (event.etype == DICE_CLICKED_EVENT) {
            // player can't swap for ai
            if (gb->activePlayer != gb->aiPlayer) {
                doDiceSwap(gb);
            }
        }

        // if (event.etype == AI_PIP_CLICKED_EVENT) {
        //     // ai shouldn't cheat ;)
        //     if (gb->activePlayer == gb->aiPlayer) {
        //         Pip* pip = (Pip*)event.ctx;
        //         doPlayerMove(gb, pip);
        //     }
        // }

        if (event.etype == UNDO_MOVE_EVENT) {
            loadCheckerState(gb);
            fsm_transition(PLAYER_MOVE_STATE);
        }
    }
}

void player_move_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: PLAYER_MOVE");
    initBoardForPlayerMove(gb);
    saveCheckerState(gb);

    if (gb->activePlayer == gb->aiPlayer) {
        // queue up moves
        GameMoveSequence gms;
        findBestMoveSequence(gb, gb->aiPlayer, &gms);
        Sint32 i = 0;
        Sint32 delay = 500;

        if (gms.numMoves > 0) {
            if (gms.swapDice) {
                SDL_AddTimer(delay, timeDoAiSwapDice, gb);
                delay += 500;
            }
            for (i = 0; i < gms.numMoves; i++) {
                GameBoardMove* gbm = SDL_malloc(sizeof(GameBoardMove));
                gbm->gb = gb;
                gbm->move = gms.moves[i];
                SDL_AddTimer(delay, timerDoAiMove, gbm);
                delay += 500;
            }
        } else {
            // end the ai turn after showing the noMoves symbol.
            SDL_AddTimer(delay, timerSetNoMovesVisible, gb);
            delay += 1000;
        }
        // end ai turn
        SDL_AddTimer(delay, timerEndAiMove, gb);
    } else {
        // timeout the player if no moves.
        SDL_AddTimer(500, timerEndPlayerTurnIfNoMoves, gb);
    }
}
