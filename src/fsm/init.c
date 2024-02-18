#include "fsm.h"
#include "game.h"
#include <stdio.h>

void init_state(FiniteStateMachine* fsm) {
    log_debug("Entered state: INIT");
    GameBoard* gb = &fsm->gb;
    initBoardForInit(gb);
    fsm_transition(WAIT_FOR_ROLL_STATE);
}

void init_init_state(FiniteStateMachine* fsm) {
    (void)fsm;
    // the init function never gets called for the first state, so this
    // function is left blank.
}
