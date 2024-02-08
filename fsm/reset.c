#include "fsm.h"
#include <stdio.h>
void reset_state_function(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx) {
    StopWatchData* stopwatchdata = (StopWatchData*)ctx;
    EventData event;
    while (fsm_dequeue_event(event_queue, &event)) {
        }
}