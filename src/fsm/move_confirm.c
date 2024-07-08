/**
 * @file move_confirm.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "fsm.h"
#include "game.h"
#include <stdio.h>

bool handleEnteredMoveConfirmEvent(GameBoard* gb) {
    initBoardForConfirm(gb);
    return false;
}

bool handleConfirmButtonClickedEvent(GameBoard* gb) {
    toggleActivePlayer(gb);
    fsm_transition(WAIT_FOR_ROLL_STATE);
    return true;
}

bool _handleUndoButtonClickedEvent(GameBoard* gb) {
    loadCheckerState(gb);
    fsm_transition(PLAYER_MOVE_STATE);
    return true;
}

void move_confirm_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    bool quit = false;
    while (!quit && fsm_dequeue_event(&event)) {
        switch (event.etype) {
        case ENTERED_MOVE_CONFIRM_STATE_EVENT:
            quit = handleEnteredMoveConfirmEvent(gb);
            break;
        case CONFIRM_BUTTON_CLICKED_EVENT:
            quit = handleConfirmButtonClickedEvent(gb);
            break;
        case UNDO_BUTTON_CLICKED_EVENT:
            quit = _handleUndoButtonClickedEvent(gb);
            break;
        default:
            break;
        }
    }
}
