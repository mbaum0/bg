
#include "fsm.h"
#include <stdlib.h>
#include <stdio.h>

void fsm_init(FiniteStateMachine *fsm) {
    
    fsm->state_functions[WAIT_FOR_ROLL_STATE] = wait_for_roll_state;
    fsm->state_init_functions[WAIT_FOR_ROLL_STATE] = wait_for_roll_init_state;
    fsm->state_functions[PLAYER_MOVE_STATE] = player_move_state;
    fsm->state_init_functions[PLAYER_MOVE_STATE] = player_move_init_state;
    fsm->state_functions[MOVE_CONFIRM_STATE] = move_confirm_state;
    fsm->state_init_functions[MOVE_CONFIRM_STATE] = move_confirm_init_state;
    fsm->state_functions[GAME_OVER_STATE] = game_over_state;
    fsm->state_init_functions[GAME_OVER_STATE] = game_over_init_state;
    fsm->current_state = WAIT_FOR_ROLL_STATE;
    fsm->eventQueue.front = 0;
    fsm->eventQueue.rear = 0;
}

void fsm_enqueue_event(FiniteStateMachine *fsm, Event event) {
    if ((fsm->eventQueue.rear + 1) % MAX_EVENTS == fsm->eventQueue.front) {
        // event queue overflow
        return;
    }
    fsm->eventQueue.events[fsm->eventQueue.rear].event_type = event;
    fsm->eventQueue.rear = (fsm->eventQueue.rear + 1) % MAX_EVENTS;
}

bool fsm_dequeue_event(FiniteStateMachine *fsm, EventData* event) {
    if (fsm->eventQueue.front == fsm->eventQueue.rear) {
        return false; // Event queue is empty
    }
    *event = fsm->eventQueue.events[fsm->eventQueue.front];
    fsm->eventQueue.front = (fsm->eventQueue.front + 1) % MAX_EVENTS;
    return true;
}

void fsm_step(FiniteStateMachine* fsm, void* ctx) {
    fsm->state_functions[fsm->current_state](fsm, ctx);
}

void fsm_transition(FiniteStateMachine *fsm, State next_state, void* ctx) {
    // Here you can add any transition logic if needed
    // For now, we'll simply update the current state
    // based on the next state provided
    // You may want to add additional logic here
    // such as handling state entry/exit actions
    fsm->state_init_functions[next_state](fsm, ctx);
    fsm->current_state = next_state;
}