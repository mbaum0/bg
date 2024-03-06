/**
 * @file wait_for_roll.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "fsm.h"
#include "game.h"
#include <stdio.h>

void wait_for_roll_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    while (fsm_dequeue_event(&event)) {
        if (event.etype == ROLL_DICE_EVENT) {
            // player can't roll for the ai
            if (gb->activePlayer != gb->aiPlayer) {
                rollDice(gb);
                fsm_transition(PLAYER_MOVE_STATE);
                break;
            }
        }

        if (event.etype == AI_ROLL_DICE_EVENT) {
            // ai shouldn't cheat ;)
            if (gb->activePlayer == gb->aiPlayer) {
                rollDice(gb);
                FSMEvent e = {AI_END_ROLL_EVENT, 0, NULL};
                fsm_enqueue_event_delay(500, e);
            }
        }

        if (event.etype == AI_END_ROLL_EVENT) {
            fsm_transition(PLAYER_MOVE_STATE);
            break;
        }

        if (event.etype == SHOW_ROLL_BTN_EVENT) {
            gb->roll.visible = true;
        }
    }
}
void wait_for_roll_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: WAIT_FOR_ROLL");
    initBoardForDiceRoll(gb);

    // auto-roll for the ai
    if (gb->activePlayer == gb->aiPlayer) {
        FSMEvent e = {AI_ROLL_DICE_EVENT, 0, NULL};
        fsm_enqueue_event_delay(500, e);
    } else {
        // only players need the roll btn. delay for aesthetics
        FSMEvent e = {SHOW_ROLL_BTN_EVENT, 0, NULL};
        fsm_enqueue_event_delay(300, e);
    }
}
