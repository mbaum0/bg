/**
 * @file wait_for_roll.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "fsm.h"
#include "game.h"
#include <stdio.h>

bool handleEnteredWaitForRollEvent(GameBoard* gb) {
    if (gb->activePlayer == gb->aiPlayer) {
        // auto roll for the ai
        rollDice(gb);
        FSMEvent e = {DICE_ROLLED_EVENT, 0, NULL};
        fsm_enqueue_event_delay(500, e);
    } else {
        gb->roll.visible = true;
    }
    return false;
}

bool handleDiceRolledEvent(GameBoard* gb) {
    (void)gb;
    gb->roll.visible = false;
    FSMEvent e = {FINISH_WAIT_FOR_ROLL_EVENT, 0, NULL};
    fsm_enqueue_event_delay(500, e);
    return false;
}

bool handleRollButtonClickedEvent(GameBoard* gb) {
    // player can't roll for ai
    if (gb->activePlayer != gb->aiPlayer) {
        rollDice(gb);
        FSMEvent e = {DICE_ROLLED_EVENT, 0, NULL};
        fsm_enqueue_event_delay(500, e);
    }
    return false;
}

bool handleFinishWaitForRollEvent(GameBoard* gb) {
    (void)gb;
    fsm_transition(PLAYER_MOVE_STATE);
    return true;
}

void wait_for_roll_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    bool quit = false;
    while (fsm_dequeue_event(&event) && !quit) {
        switch (event.etype) {
        case ENTERED_WAIT_FOR_ROLL_STATE_EVENT:
            quit = handleEnteredWaitForRollEvent(gb);
            break;
        case ROLL_BUTTON_CLICKED_EVENT:
            quit = handleRollButtonClickedEvent(gb);
            break;
        case DICE_ROLLED_EVENT:
            quit = handleDiceRolledEvent(gb);
            break;
        case FINISH_WAIT_FOR_ROLL_EVENT:
            quit = handleFinishWaitForRollEvent(gb);
        default:
            break;
        }
    }
}
void wait_for_roll_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: WAIT_FOR_ROLL");
    initBoardForDiceRoll(gb);

    FSMEvent e = {ENTERED_WAIT_FOR_ROLL_STATE_EVENT, 0, NULL};
    fsm_enqueue_event_delay(300, e);
}
