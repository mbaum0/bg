
#include "fsm.h"
#include <stdio.h>
#include <stdlib.h>

FiniteStateMachine FSM = {0};

const char* getStateName(State state) {
    static const char* stateNames[] = {
#define GENERATE_STRING(STRING) #STRING,
        FOREACH_STATE(GENERATE_STRING)
#undef GENERATE_STRING
    };

    if (state >= 0 && state < NUM_STATES)
        return stateNames[state];
    return "Unknown";
}

const char* getFsmEventName(FSMEventType event) {
    static const char* eventNames[] = {
#define GENERATE_STRING(STRING) #STRING,
        FOREACH_EVENT(GENERATE_STRING)
#undef GENERATE_STRING
    };

    if (event >= 0 && event < NUM_EVENTS)
        return eventNames[event];
    return "Unknown";
}

void fsm_init(void) {
    FSM.state_functions[INIT_STATE] = init_state;
    FSM.state_functions[ROLL_FOR_FIRST_STATE] = roll_for_first_state;
    FSM.state_functions[WAIT_FOR_ROLL_STATE] = wait_for_roll_state;
    FSM.state_functions[PLAYER_MOVE_STATE] = player_move_state;
    FSM.state_functions[MOVE_CONFIRM_STATE] = move_confirm_state;
    FSM.state_functions[ROUND_OVER_STATE] = round_over_state;
    FSM.current_state = INIT_STATE;
    FSM.eventQueue.front = 0;
    FSM.eventQueue.rear = 0;
}

Uint32 delay_enqueue_event(void* userdata, SDL_TimerID timerID, Uint32 interval) {
    (void)interval;
    (void)timerID;
    FSMEvent* e = (FSMEvent*)userdata;
    fsm_enqueue_event(*e);
    SDL_free(e);
    return 0;
}

void fsm_enqueue_event_delay(Uint32 delay, FSMEvent e) {
    FSMEvent* newEvent = SDL_malloc(sizeof(FSMEvent));
    *newEvent = e;
    SDL_AddTimer(delay, delay_enqueue_event, newEvent);
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
    log_debug("Got event: %s", getFsmEventName(event->etype));
    return true;
}

void fsm_step(void) {
    FSM.state_functions[FSM.current_state](&FSM);
}

void fsm_transition(State next_state) {
    // empty out the queue
    FSMEvent e;
    while (fsm_dequeue_event(&e))
        ;

    // we can cast next_state -> FSMEventType here because
    // they align in enum values.
    e = (FSMEvent){(FSMEventType)next_state, 0, NULL};
    fsm_enqueue_event(e);
    FSM.current_state = next_state;
    log_debug("Entered state: %s", getStateName(next_state));
}

void save_state(FiniteStateMachine* fsm) {
    FILE* file = fopen("state.save", "wb"); // "wb" mode for writing in binary format
    if (file == NULL) {
        log_error("Error opening file");
        return;
    }

    fwrite(&fsm->gb, sizeof(GameBoard), 1, file);
    fwrite(&fsm->current_state, sizeof(State), 1, file);
    fclose(file);
}

void load_state(FiniteStateMachine* fsm) {
    FILE* file = fopen("state.save", "rb"); // "rb" mode for reading in binary format
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
