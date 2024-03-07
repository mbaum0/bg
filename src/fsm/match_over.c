#include "fsm.h"
#include <stdio.h>

bool handleEnteredMatchOverStateEvent(GameBoard* gb) {
    gb->dialog.visible = true;
    gb->dialog.alpha = 0;
    gb->dialog.moving = true;
    return false;
}
void match_over_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    bool quit = false;
    while (!quit && fsm_dequeue_event(&event)) {
        switch (event.etype) {
        case ENTERED_MATCH_OVER_STATE_EVENT:
            quit = handleEnteredMatchOverStateEvent(gb);
            break;
        default:
            break;
        }
    }
}
