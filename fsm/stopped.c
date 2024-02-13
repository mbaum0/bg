#include "fsm.h"
#include <stdio.h>
void stopped_state_function(FiniteStateMachine* fsm, void* ctx) {
    StopWatchData* stopwatchdata = (StopWatchData*)ctx;
    EventData event;
    while (fsm_dequeue_event(fsm, &event)) {
        if (event.event_type == START_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
            fsm_transition(fsm, RUNNING_STATE, ctx);
        }
        if (event.event_type == RESET_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
            stopwatchdata->counter = 0;
            printf("RESET: %d\n", stopwatchdata->counter);
        }
        
    }
    
}
void stopped_init_state_function(FiniteStateMachine* fsm, void* ctx) {
    StopWatchData* stopwatchdata = (StopWatchData*)ctx;
    // this is called when the stopped state is first entered
    printf("Transitioned to STOPPED\n");
}