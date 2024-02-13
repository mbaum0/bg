#include "fsm.h"
#include <stdio.h>
void stopped_state_function(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx) {
    StopWatchData* stopwatchdata = (StopWatchData*)ctx;
    EventData event;
    while (fsm_dequeue_event(event_queue, &event)) {
        if (event.event_type == START_EVENT) {
            // Transition to a different state here
            fsm_transition(fsm, RUNNING_STATE);
            printf("Started\n");
        }
        if (event.event_type == RESET_EVENT) {
            // Transition to a different state here
            //fsm_transition(fsm, RESET_EVENT);
            stopwatchdata->counter = 0;
            printf("Reset\n");
        }
    }
    
}