/**
 * @file wait_for_roll.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "fsm.h"
#include "game.h"
#include <stdio.h>

uint32_t timerAiRoll(uint32_t interval, void* ctx) {
    (void)interval;
    (void)ctx;
    FSMEvent e = {AI_DICE_CLICKED_EVENT, NULL};
    fsm_enqueue_event(e);
    return 0;
}

void wait_for_roll_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    while (fsm_dequeue_event(&event)) {
        if (event.etype == DICE_CLICKED_EVENT) {
            // player can't roll for the ai
            if (gb->activePlayer != gb->aiPlayer) {
                rollDice(gb);
                fsm_transition(PLAYER_MOVE_STATE);
            }
        }

        if (event.etype == AI_DICE_CLICKED_EVENT) {
            // ai shouldn't cheat ;)
            if (gb->activePlayer == gb->aiPlayer) {
                rollDice(gb);
                fsm_transition(PLAYER_MOVE_STATE);
            }
        }
    }
}
void wait_for_roll_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: WAIT_FOR_ROLL");
    updateBoardForDiceRoll(gb);

    // auto-roll for the ai
    if (gb->activePlayer == gb->aiPlayer) {
        SDL_AddTimer(500, timerAiRoll, gb);
    }
}
