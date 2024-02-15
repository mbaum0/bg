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

uint32_t timerEndPlayerTurnIfNoMoves(uint32_t interval, void* ctx) {
    (void)interval;
    GameBoard* gb = (GameBoard*)ctx;
    GameDie* die1 = FIRST_DIE(gb);
    bool hasBothDiceLeft = (die1->uses == 0);

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

void doAiMove(GameBoard* gb, GameMove gm) {
    Checker* c = getTopCheckerOnPip(gb, gm.srcPip);
    if (c == NULL) {
        log_error("AI picked an empty pip.");
        return;
    }

    int32_t dieValue = getNextDieValue(gb);

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

uint32_t timerDoAiMove(uint32_t interval, void* ctx) {
    (void)interval;
    GameBoardMove* gbm = (GameBoardMove*)ctx;
    doAiMove(gbm->gb, gbm->move);
    SDL_free(gbm);
    return 0;
}

uint32_t timerEndAiMove(uint32_t interval, void* ctx) {
    (void)interval;
    GameBoard* gb = (GameBoard*)ctx;
    gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
    fsm_transition(WAIT_FOR_ROLL_STATE);
    return 0;
}

void doPlayerMove(GameBoard* gb, int32_t pipIndex) {
    int32_t movesLeft = 0;
    Checker* c = getTopCheckerOnPip(gb, pipIndex);
    if (c == NULL) {
        return;
    }

    int32_t dieValue = getNextDieValue(gb);
    GameMove gm = {c->color, c->pipIndex, dieValue};

    if (isValidMove(gb, gm)) {
        moveChecker(gb, gm);
        movesLeft = incrementMoveCount(gb);
    } else {
        return;
    }

    if (movesLeft == 0) {
        fsm_transition(MOVE_CONFIRM_STATE);
    }
    SDL_AddTimer(1000, timerEndPlayerTurnIfNoMoves, gb);
}

void doDiceSwap(GameBoard* gb) {
    swapDiceIfAllowed(gb);
}

void player_move_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    while (fsm_dequeue_event(&event)) {
        if (event.etype == PIP_CLICKED_EVENT) {
            // player can't pick for ai
            if (gb->activePlayer != gb->aiPlayer) {
                uint32_t pipIndex = (uintptr_t)event.ctx;
                doPlayerMove(gb, pipIndex);
            }
        }

        if (event.etype == DICE_CLICKED_EVENT) {
            // player can't swap for ai
            if (gb->activePlayer != gb->aiPlayer) {
                doDiceSwap(gb);
            }
        }

        if (event.etype == AI_PIP_CLICKED_EVENT) {
            // ai shouldn't cheat ;)
            if (gb->activePlayer == gb->aiPlayer) {
                uint32_t pipIndex = (uintptr_t)event.ctx;
                doPlayerMove(gb, pipIndex);
            }
        }
    }
}

void player_move_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: PLAYER_MOVE");
    updateBoardForPlayerMove(gb);
    saveCheckerState(gb);

    if (gb->activePlayer == gb->aiPlayer) {
        // queue up moves
        GameMoveSequence gms = findBestMoveSequence(gb, gb->aiPlayer);
        int32_t i = 0;
        int32_t delay = 500;
        for (i = 0; i < gms.numMoves; i++) {
            GameBoardMove* gbm = SDL_malloc(sizeof(GameBoardMove));
            gbm->gb = gb;
            gbm->move = gms.moves[i];
            SDL_AddTimer(delay, timerDoAiMove, gbm);
            delay += 500;
        }
        SDL_AddTimer(delay, timerEndAiMove, gb);
    } else {
        // timeout the player if no moves.
        SDL_AddTimer(500, timerEndPlayerTurnIfNoMoves, gb);
    }
}
