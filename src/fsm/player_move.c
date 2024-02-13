/**
 * @file player_move.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#include "fsm.h"
#include "game.h"
#include <stdio.h>

void doPlayerMove(GameBoard* gb, int32_t pipIndex){
    int32_t movesLeft = 0;
    Checker* c = getTopCheckerOnPip(gb, pipIndex);
    if (c == NULL){
        return;
    }

    int32_t dieValue = getNextDieValue(gb);
    if (isValidMove(gb, c, dieValue)){
        moveChecker(gb, c, dieValue);
        movesLeft = incrementMoveCount(gb);
    } else {
        return;
    }

    if (movesLeft == 0){
        fsm_transition(MOVE_CONFIRM_STATE);
    }
}

void doDiceSwap(GameBoard* gb){
    swapDiceIfAllowed(gb);
}

void player_move_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    while (fsm_dequeue_event(&event)) {
        if (event.etype == PIP_CLICKED_EVENT) {
            uint32_t pipIndex = (uintptr_t)event.ctx;
            doPlayerMove(gb, pipIndex);
        }

        if (event.etype == DICE_CLICKED_EVENT) {
            doDiceSwap(gb);
        }   
    }
}
void player_move_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: PLAYER_TURN");
    updateBoardForPlayerMove(gb);
    saveCheckerState(gb);
}
