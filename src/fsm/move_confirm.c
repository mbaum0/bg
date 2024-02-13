#include "fsm.h"
#include <stdio.h>
void move_confirm_state(FiniteStateMachine* fsm, void* ctx) {
    GameBoard* gameboard = (GameBoard*)ctx;
    EventData event;
    while (fsm_dequeue_event(fsm, &event)) {
        if (event.event_type == CONFIRMED_MOVE_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
        }
        if (event.event_type == UNDO_MOVE_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
        }
        
    }
    
}
void move_confirm_init_state(FiniteStateMachine* fsm, void* ctx) {
    GameBoard* gameboard = (GameBoard*)ctx;
    // this is called when the move_confirm state is first entered
}