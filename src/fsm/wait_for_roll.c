#include "fsm.h"
#include <stdio.h>
void wait_for_roll_state(FiniteStateMachine* fsm, void* ctx) {
    GameBoard* gameboard = (GameBoard*)ctx;
    EventData event;
    while (fsm_dequeue_event(fsm, &event)) {
        if (event.event_type == DICE_ROLL_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
        }
    }
    
}
void wait_for_roll_init_state(FiniteStateMachine* fsm, void* ctx) {
    GameBoard* gameboard = (GameBoard*)ctx;
    // this is called when the wait_for_roll state is first entered
}