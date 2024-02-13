
#include "fsm.h"
#include <stdlib.h>
#include <stdio.h>

FiniteStateMachine FSM = { 0 };

void fsm_init(void) {
    
    FSM.state_functions[WAIT_FOR_ROLL_STATE] = wait_for_roll_state;
    FSM.state_init_functions[WAIT_FOR_ROLL_STATE] = wait_for_roll_init_state;
    FSM.state_functions[PLAYER_MOVE_STATE] = player_move_state;
    FSM.state_init_functions[PLAYER_MOVE_STATE] = player_move_init_state;
    FSM.state_functions[MOVE_CONFIRM_STATE] = move_confirm_state;
    FSM.state_init_functions[MOVE_CONFIRM_STATE] = move_confirm_init_state;
    FSM.state_functions[GAME_OVER_STATE] = game_over_state;
    FSM.state_init_functions[GAME_OVER_STATE] = game_over_init_state;
    FSM.current_state = WAIT_FOR_ROLL_STATE;
    FSM.eventQueue.front = 0;
    FSM.eventQueue.rear = 0;
}

void fsm_enqueue_event(FSMEvent event) {
    if ((FSM.eventQueue.rear + 1) % MAX_EVENTS == FSM.eventQueue.front) {
        // event queue overflow
        return;
    }
    FSM.eventQueue.events[FSM.eventQueue.rear] = event;
    FSM.eventQueue.rear = (FSM.eventQueue.rear + 1) % MAX_EVENTS;
}

bool fsm_dequeue_event(FSMEvent* event) {
    if (FSM.eventQueue.front == FSM.eventQueue.rear) {
        return false; // Event queue is empty
    }
    *event = FSM.eventQueue.events[FSM.eventQueue.front];
    FSM.eventQueue.front = (FSM.eventQueue.front + 1) % MAX_EVENTS;
    return true;
}

void fsm_step(void) {
    FSM.state_functions[FSM.current_state](&FSM);
}

void fsm_transition(State next_state) {
    FSM.state_init_functions[next_state](&FSM);
    FSM.current_state = next_state;
}
