
#include "fsm.h"
#include <stdlib.h>
#include <stdio.h>

void fsm_init(FiniteStateMachine *fsm) {
    
    fsm->state_functions[STOPPED_STATE] = stopped_state_function;
    fsm->state_functions[RUNNING_STATE] = running_state_function;
    fsm->state_functions[RESET_STATE] = reset_state_function;
    fsm->current_state = STOPPED_STATE;
}

void fsm_enqueue_event(EventQueue* queue, Event event) {
    if ((queue->rear + 1) % MAX_EVENTS == queue->front) {
        // event queue overflow
        return;
    }
    queue->events[queue->rear].event_type = event;
    queue->rear = (queue->rear + 1) % MAX_EVENTS;
}

bool fsm_dequeue_event(EventQueue* queue, EventData* event) {
    if (queue->front == queue->rear) {
        return false; // Event queue is empty
    }
    *event = queue->events[queue->front];
    queue->front = (queue->front + 1) % MAX_EVENTS;
    return true;
}

void fsm_step(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx) {
    fsm->state_functions[fsm->current_state](fsm, event_queue, ctx);
}

void fsm_transition(FiniteStateMachine *fsm, State next_state) {
    // Here you can add any transition logic if needed
    // For now, we'll simply update the current state
    // based on the next state provided
    // You may want to add additional logic here
    // such as handling state entry/exit actions
    fsm->current_state = next_state;
}