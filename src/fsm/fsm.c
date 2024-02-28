
#include "fsm.h"
#include <stdio.h>
#include <stdlib.h>

FiniteStateMachine FSM = {0};

void fsm_init(void) {
    FSM.state_functions[INIT_STATE] = init_state;
    FSM.state_init_functions[INIT_STATE] = init_init_state;
    FSM.state_init_functions[ROLL_FOR_FIRST_STATE] = roll_for_first_init_state;
    FSM.state_functions[ROLL_FOR_FIRST_STATE] = roll_for_first_state;
    FSM.state_functions[WAIT_FOR_ROLL_STATE] = wait_for_roll_state;
    FSM.state_init_functions[WAIT_FOR_ROLL_STATE] = wait_for_roll_init_state;
    FSM.state_functions[PLAYER_MOVE_STATE] = player_move_state;
    FSM.state_init_functions[PLAYER_MOVE_STATE] = player_move_init_state;
    FSM.state_functions[MOVE_CONFIRM_STATE] = move_confirm_state;
    FSM.state_init_functions[MOVE_CONFIRM_STATE] = move_confirm_init_state;
    FSM.state_functions[GAME_OVER_STATE] = game_over_state;
    FSM.state_init_functions[GAME_OVER_STATE] = game_over_init_state;
    FSM.current_state = INIT_STATE;
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

void save_state(FiniteStateMachine* fsm){
    FILE *file = fopen("state.save", "wb"); // "wb" mode for writing in binary format
    if (file == NULL) {
        log_error("Error opening file");
        return;
    }

    fwrite(&fsm->gb, sizeof(GameBoard), 1, file);
    fwrite(&fsm->current_state, sizeof(State), 1, file);
    fclose(file);
}

void load_state(FiniteStateMachine* fsm){
    FILE *file = fopen("state.save", "rb"); // "rb" mode for reading in binary format
    if (file == NULL) {
        log_error("Error opening file");
        return;
    }

    if (fread(&fsm->gb, sizeof(GameBoard), 1, file) != 1) {
        log_error("Error reading game board.");
        goto close;
    }

    if (fread(&fsm->current_state, sizeof(State), 1, file) != 1) {
        log_error("Error reading state.");
        goto close;
    }
close:
    fclose(file);
}
