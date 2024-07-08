#include "fsm.h"
#include <stdio.h>

bool handleEnteredRoundOverStateEvent(GameBoard* gb) {
    Player winner = getRoundWinner(gb);
    if (winner == PLAYER_ONE) {
        gb->dialog.roundsWon++;
        gb->dialog.playerWon = true;
    } else if (winner == PLAYER_TWO) {
        gb->dialog.roundsLost++;
        gb->dialog.playerWon = false;
    } else {
        log_warn("Entered round_over state with no winner.");
    }
    gb->dialog.visible = true;
    gb->dialog.alpha = 0;
    gb->dialog.moving = true;
    return false;
}

bool handleNextRoundButtonClickedEvent(GameBoard* gb) {
    gameboard_reset(gb);
    fsm_transition(ROLL_FOR_FIRST_STATE);
    return true;
}

void round_over_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    bool quit = false;
    while (!quit && fsm_dequeue_event(&event)) {
        switch (event.etype) {
        case ENTERED_ROUND_OVER_STATE_EVENT:
            quit = handleEnteredRoundOverStateEvent(gb);
            break;
        case NEXT_ROUND_BUTTON_CLICKED_EVENT:
            quit = handleNextRoundButtonClickedEvent(gb);
        default:
            break;
        }
    }
}
