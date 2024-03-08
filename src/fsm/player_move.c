/**
 * @file player_move.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "ai.h"
#include "fsm.h"
#include "game.h"
#include <stdio.h>

bool doAiMove(GameBoard* gb, GameMove gm) {
    Checker* c = getTopCheckerOnPip(gb, gm.srcPip);
    if (c == NULL) {
        log_error("AI picked an empty pip.");
        return false;
    }

    Sint32 dieValue = getNextDieValue(gb);

    if (dieValue != gm.amount) {
        log_error("AI picked invalid die value.");
        return false;
    }

    if (isValidMove(gb, gm)) {
        moveChecker(gb, gm);
        log_debug("moved checker from pip %d to pip %d", gm.srcPip, gm.srcPip + gm.amount);
        incrementMoveCount(gb);
    } else {
        log_error("AI picked an invalid move.");
    }
    return false;
}

bool doPlayerMove(GameBoard* gb, Uint32 pipIndex) {
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
        log_debug("moved checker from pip %d to pip %d", gm.srcPip, gm.srcPip - gm.amount);
        movesLeft = incrementMoveCount(gb);
        success = true;
    } else {
        success = false;
        goto final;
    }

    if (movesLeft == 0 || !playerHasMoves(gb)) {
        FSMEvent e = {FINISHED_PLAYER_MOVE_EVENT, 0, NULL};
        fsm_enqueue_event_delay(500, e);
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
    return false;
}

void doDiceSwap(GameBoard* gb) {
    swapDiceIfAllowed(gb);
}

void doAiTurn(GameBoard* gb) {

    // get the best move sequence
    GameMoveSequence gms;
    findBestMoveSequence(gb, gb->aiPlayer, &gms);

    Sint32 i = 0;
    Sint32 delay = 100;

    if (gms.numMoves > 0) {
        if (gms.swapDice) {
            FSMEvent e = {AI_SWAP_DICE_EVENT, 0, NULL};
            fsm_enqueue_event_delay(delay, e);
            delay += 400;
        }
        // do each move with a delay
        for (i = 0; i < gms.numMoves; i++) {
            GameMove* gm = SDL_malloc(sizeof(GameMove));
            // gbm->gb = gb;
            *gm = gms.moves[i];
            // SDL_AddTimer(delay, timerDoAiMove, gbm);
            FSMEvent e = {AI_MOVE_EVENT, 0, gm};
            fsm_enqueue_event_delay(delay, e);
            delay += 400;
        }
    } else {
        // end the ai turn after showing the noMoves symbol.
        gb->nomoves.visible = true;
        delay += 500;
    }

    // end ai turn
    delay += 200;
    FSMEvent e = {FINISHED_PLAYER_MOVE_EVENT, 0, NULL};
    fsm_enqueue_event_delay(delay, e);
}

bool handleEnteredPlayerMoveEvent(GameBoard* gb) {
    initBoardForPlayerMove(gb);
    saveCheckerState(gb);

    if (gb->activePlayer == gb->aiPlayer) {
        doAiTurn(gb);
    } else {
        if (!playerHasMoves(gb)) {
            gb->nomoves.visible = true;
            FSMEvent e = {FINISHED_PLAYER_MOVE_EVENT, 0, NULL};
            fsm_enqueue_event_delay(500, e);
            return true;
        }
    }
    return false;
}

bool handlePipClickedEvent(GameBoard* gb, Uint32 pipIndex) {
    if (gb->activePlayer != gb->aiPlayer) {
        doPlayerMove(gb, pipIndex);
    }
    return false;
}

bool handleDiceClickedEvent(GameBoard* gb) {
    if (gb->activePlayer != gb->aiPlayer) {
        doDiceSwap(gb);
    }
    return false;
}

bool handleUndoButtonClickedEvent(GameBoard* gb) {
    loadCheckerState(gb);
    fsm_transition(PLAYER_MOVE_STATE);
    return true;
}

bool handleAiMoveEvent(GameBoard* gb, GameMove* gm) {
    doAiMove(gb, *gm);
    SDL_free(gm);
    return false;
}

bool handleAiSwapDiceEvent(GameBoard* gb) {
    swapDiceIfAllowed(gb);
    return false;
}

bool handleFinishedPlayerMoveEvent(GameBoard* gb) {
    gb->nomoves.visible = false;

    if (getMatchWinner(gb) != NONE) {
        fsm_transition(MATCH_OVER_STATE);
        return true;
    }

    // ai will always go to next turn
    if (gb->activePlayer == gb->aiPlayer) {
        gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
        fsm_transition(WAIT_FOR_ROLL_STATE);

        // if player had no moves after dice roll, go right into next player's turn
    } else if (!playerHasMoves(gb) && !haveDiceBeenUsed(gb)) {
        gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
        fsm_transition(WAIT_FOR_ROLL_STATE);

        // let player confirm their move
    } else {
        fsm_transition(MOVE_CONFIRM_STATE);
    }
    return true;
}

void player_move_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    updateBoardForPlayerMove(gb);
    bool quit = false;
    while (!quit && fsm_dequeue_event(&event)) {

        switch (event.etype) {
        case ENTERED_PLAYER_MOVE_STATE_EVENT:
            quit = handleEnteredPlayerMoveEvent(gb);
            break;
        case PIP_CLICKED_EVENT:
            quit = handlePipClickedEvent(gb, event.code);
            break;
        case DICE_CLICKED_EVENT:
            quit = handleDiceClickedEvent(gb);
            break;
        case UNDO_BUTTON_CLICKED_EVENT:
            quit = handleUndoButtonClickedEvent(gb);
            break;
        case FINISHED_PLAYER_MOVE_EVENT:
            quit = handleFinishedPlayerMoveEvent(gb);
            break;
        case AI_MOVE_EVENT:
            quit = handleAiMoveEvent(gb, event.ctx);
            break;
        case AI_SWAP_DICE_EVENT:
            quit = handleAiSwapDiceEvent(gb);
            break;
        default:
            break;
        }
    }
}
