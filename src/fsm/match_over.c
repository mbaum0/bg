#include "fsm.h"
#include <stdio.h>
void match_over_state(FiniteStateMachine* fsm) {
    (void)fsm;
    GameBoard* gb = &fsm->gb;
    gb->dialog.visible = true;
}
