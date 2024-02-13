/**
 * @file move_confirm.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#include "fsm.h"
#include "game.h"
#include <stdio.h>

void doConfirmMove(GameBoard* gb){
    gb->activePlayer = OPPONENT_COLOR(gb->activePlayer);
}
void move_confirm_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    EventData event;
    while (fsm_dequeue_event(&event)) {
        if (event.event_type == CONFIRMED_MOVE_EVENT) {
            // Transition to a different state here
            doConfirmMove(gb);
            fsm_transition(WAIT_FOR_ROLL_STATE);
        }
        if (event.event_type == UNDO_MOVE_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
        }
        
    }
    
}
void move_confirm_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: CONFIRM");
    updateBoardForConfirm(gb);
}
