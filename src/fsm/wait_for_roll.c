/**
 * @file wait_for_roll.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#include "fsm.h"
#include "game.h"
#include <stdio.h>

extern FiniteStateMachine FSM;

void wait_for_roll_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    EventData event;
    while (fsm_dequeue_event(&event)) {
        if (event.event_type == DICE_ROLL_EVENT) {
            rollDice(gb);
            fsm_transition(PLAYER_MOVE_STATE);
        }
    }
    
}
void wait_for_roll_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: WAIT_FOR_ROLL");
    updateBoardForDiceRoll(gb);
}
