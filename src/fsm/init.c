#include "fsm.h"
#include "game.h"
#include <stdio.h>

void init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    initBoardForInit(gb);
    fsm_transition(ROLL_FOR_FIRST_STATE);
}
