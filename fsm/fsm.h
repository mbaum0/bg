#pragma once
#include <stdbool.h>

typedef enum {
    START_EVENT,
    RESET_EVENT,
    STOP_EVENT,
    RESET_EVENT
} Event;

typedef struct {
    Event event_type;
} EventData;

typedef enum {
    STOPPED_STATE,
    RUNNING_STATE,
    RESET_STATE,
    NUM_STATES
} State;
// Define event queue
#define MAX_EVENTS 10
typedef struct {
    EventData events[MAX_EVENTS];
    int front, rear;
} EventQueue;

typedef struct FiniteStateMachine {
    State current_state;
    void (*state_functions[NUM_STATES])(struct FiniteStateMachine*, EventQueue*, void*);
} FiniteStateMachine;

void fsm_init(FiniteStateMachine* fsm);
void fsm_enqueue_event(EventQueue* queue, Event event);
bool fsm_dequeue_event(EventQueue* queue, EventData* event);
void fsm_step(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx);
void fsm_transition(FiniteStateMachine *fsm, State next_state);

typedef struct {} StopWatchData;
void stopped_state_function(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx);
void running_state_function(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx);
void reset_state_function(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx);
