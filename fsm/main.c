#include "fsm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

int main(void) {
    FiniteStateMachine fsm;
    EventQueue event_queue;
    StopWatchData swd;
    fsm_init(&fsm);
    event_queue.front = 0;
    event_queue.rear = 0;

    printf("Enter '1' to start, '2' to stop, '3' to reset, and '4' to step the stopwatch.\n");

    while (1) {
        char input;
        scanf(" %c", &input);

        switch (input) {
            case '1':
                fsm_enqueue_event(&event_queue, START_EVENT);
                break;
            case '2':
                fsm_enqueue_event(&event_queue, STOP_EVENT);
                break;
            case '3':
                fsm_enqueue_event(&event_queue, RESET_EVENT);
                break;
            case '4':
                fsm_enqueue_event(&event_queue, STEP_EVENT);
                break;
            default:
                printf("Invalid input. Please enter '1', '2', '3', or '4'.\n");
                continue;
        }

        fsm_step(&fsm, &event_queue, &swd);
    }

    return 0;
}