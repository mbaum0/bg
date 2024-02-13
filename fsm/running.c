#include "fsm.h"
#include <stdio.h>
void running_state_function(FiniteStateMachine* fsm, void* ctx) {
    StopWatchData* stopwatchdata = (StopWatchData*)ctx;
    EventData event;
    while (fsm_dequeue_event(fsm, &event)) {
        if (event.event_type == STOP_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
            fsm_transition(fsm, STOPPED_STATE, ctx);
        }
        if (event.event_type == RESET_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
            stopwatchdata->counter = 0;
            printf("RESET: %d\n", stopwatchdata->counter);
        }
        if (event.event_type == STEP_EVENT) {
            // Transition to a different state here
            // fsm_transition(fsm, NEW_STATE, ctx);
            stopwatchdata->counter++;
            printf("STEPPED: %d\n", stopwatchdata->counter);
        }
        
    }
    
}
void running_init_state_function(FiniteStateMachine* fsm, void* ctx) {
    StopWatchData* stopwatchdata = (StopWatchData*)ctx;
    // this is called when the running state is first entered
    printf("Transitioned to RUNNING\n");
}