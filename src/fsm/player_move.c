#include "fsm.h"
#include <stdio.h>
void player_move_state(FiniteStateMachine* fsm, void* ctx) {
    GameBoard* gameboard = (GameBoard*)ctx;
    EventData event;
    while (fsm_dequeue_event(fsm, &event)) {
        if (event.event_type == PIP_SELECTED_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
        }
        if (event.event_type == DICE_CLICKED_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
        }
        
    }
    
}
void player_move_init_state(FiniteStateMachine* fsm, void* ctx) {
    GameBoard* gameboard = (GameBoard*)ctx;
    // this is called when the player_move state is first entered
}