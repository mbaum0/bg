/**
 * @file move_confirm.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "fsm.h"
#include "game.h"
#include <stdio.h>

void doConfirmMove(GameBoard* gb) {
    gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
}
void move_confirm_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    while (fsm_dequeue_event(&event)) {
        if (event.etype == CONFIRMED_MOVE_EVENT) {
            doConfirmMove(gb);
            fsm_transition(WAIT_FOR_ROLL_STATE);
        }
        if (event.etype == UNDO_MOVE_EVENT) {
            loadCheckerState(gb);
            fsm_transition(PLAYER_MOVE_STATE);
        }
    }
}
void move_confirm_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: CONFIRM");
    initBoardForConfirm(gb);
}
