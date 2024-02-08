#include "fsm.h"
#include <stdio.h>
void running_state_function(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx) {
    StopWatchData* stopwatchdata = (StopWatchData*)ctx;
    EventData event;
    while (fsm_dequeue_event(event_queue, &event)) {
        if (event.event_type == STOP_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE);
        }
        if (event.event_type == RESET_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE);
        }
        }
}