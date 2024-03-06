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
        if (!playerHasMoves(gb)) {
            FSMEvent e = {PLAYER_HAS_NO_MOVES_EVENT, 0, NULL};
            fsm_enqueue_event_delay(1000, e);
        }
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

void doAiTurn(GameBoard* gb) {

    // get the best move sequence
    GameMoveSequence gms;
    findBestMoveSequence(gb, gb->aiPlayer, &gms);

    Sint32 i = 0;
    Sint32 delay = 500;

    if (gms.numMoves > 0) {
        if (gms.swapDice) {
            FSMEvent e = {AI_SWAP_DICE_EVENT, 0, NULL};
            fsm_enqueue_event_delay(delay, e);
            delay += 500;
        }
        // do each move with a delay
        for (i = 0; i < gms.numMoves; i++) {
            GameMove* gm = SDL_malloc(sizeof(GameMove));
            // gbm->gb = gb;
            *gm = gms.moves[i];
            // SDL_AddTimer(delay, timerDoAiMove, gbm);
            FSMEvent e = {AI_MOVE_EVENT, 0, gm};
            fsm_enqueue_event_delay(delay, e);
            delay += 500;
        }
    } else {
        // end the ai turn after showing the noMoves symbol.
        FSMEvent e = {SHOW_NO_MOVES_ICON_EVENT, 0, NULL};
        fsm_enqueue_event(e);
        delay += 1000;
    }
    // end ai turn
    FSMEvent e = {AI_END_TURN_EVENT, 0, NULL};
    fsm_enqueue_event_delay(delay, e);
}

void player_move_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    updateBoardForPlayerMove(gb);
    while (fsm_dequeue_event(&event)) {

        if (event.etype == AI_START_TURN_EVENT) {
            doAiTurn(gb);
            continue;
        }

        if (event.etype == AI_END_TURN_EVENT) {
            gb->nomoves.visible = false; // hide this in case it was made visible
            gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
            fsm_transition(WAIT_FOR_ROLL_STATE);
            break;
        }

        if (event.etype == AI_SWAP_DICE_EVENT) {
            swapDiceIfAllowed(gb);
            continue;
        }

        if (event.etype == AI_MOVE_EVENT) {
            GameMove* gm = (GameMove*)event.ctx;
            doAiMove(gb, *gm);
            SDL_free(gm);
        }

        if (event.etype == SHOW_NO_MOVES_ICON_EVENT) {
            gb->nomoves.visible = true;
            continue;
        }

        if (event.etype == PIP_CLICKED_EVENT) {
            // player can't pick for ai
            if (gb->activePlayer != gb->aiPlayer) {
                doPlayerMove(gb, event.code);
            }
            continue;
        }

        if (event.etype == DICE_CLICKED_EVENT) {
            // player can't swap for ai
            if (gb->activePlayer != gb->aiPlayer) {
                doDiceSwap(gb);
            }
            continue;
        }

        if (event.etype == UNDO_MOVE_EVENT) {
            loadCheckerState(gb);
            fsm_transition(PLAYER_MOVE_STATE);
            break;
        }

        if (event.etype == PLAYER_HAS_NO_MOVES_EVENT) {
            log_debug("Active player has no moves available");
            GameDie* die1 = FIRST_DIE(gb);
            bool hasBothDiceLeft = (die1->uses == 0);
            if (hasBothDiceLeft) {
                gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
                fsm_transition(WAIT_FOR_ROLL_STATE);
                break;
            } else {
                fsm_transition(MOVE_CONFIRM_STATE);
                break;
            }
        }
    }
}

void player_move_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: PLAYER_MOVE");
    initBoardForPlayerMove(gb);
    saveCheckerState(gb);

    if (gb->activePlayer == gb->aiPlayer) {
        FSMEvent e = {AI_START_TURN_EVENT, 0, NULL};
        fsm_enqueue_event(e);
    }

    if (gb->activePlayer != gb->aiPlayer) {
        if (!playerHasMoves(gb)) {
            FSMEvent e = {SHOW_NO_MOVES_ICON_EVENT, 0, NULL};
            fsm_enqueue_event_delay(500, e);
            e = (FSMEvent){PLAYER_HAS_NO_MOVES_EVENT, 0, NULL};
            fsm_enqueue_event_delay(1000, e);
        }
    }
}
