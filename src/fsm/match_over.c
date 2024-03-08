#include "fsm.h"
#include <stdio.h>

bool handleEnteredMatchOverStateEvent(GameBoard* gb) {
    gb->dialog.visible = true;
    gb->dialog.alpha = 0;
    gb->dialog.moving = true;

    Color winner = getMatchWinner(gb);
    if (winner == LIGHT) {
        gb->dialog.matchesWon++;
    } else if (winner == DARK) {
        gb->dialog.matchesLost++;
    } else {
        log_warn("Entered match_over state with no winner.");
    }
    return false;
}

bool handleNextMatchButtonClickedEvent(GameBoard* gb) {
    gameboard_reset(gb);
    fsm_transition(ROLL_FOR_FIRST_STATE);
    return true;
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
        case NEXT_MATCH_BUTTON_CLICKED_EVENT:
            quit = handleNextMatchButtonClickedEvent(gb);
        default:
            break;
        }
    }
}
