/**
 * @file wait_for_roll.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "fsm.h"
#include "game.h"
#include <stdio.h>

Uint32 timerAiRoll(Uint32 interval, void* ctx) {
    (void)interval;
    (void)ctx;
    FSMEvent e = {AI_ROLL_DICE_EVENT, 0, NULL};
    fsm_enqueue_event(e);
    return 0;
}

void wait_for_roll_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    FSMEvent event;
    while (fsm_dequeue_event(&event)) {
        if (event.etype == ROLL_DICE_EVENT) {
            // player can't roll for the ai
            if (gb->activePlayer != gb->aiPlayer) {
                rollDice(gb);
                fsm_transition(PLAYER_MOVE_STATE);
            }
        }

        if (event.etype == AI_ROLL_DICE_EVENT) {
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
    initBoardForDiceRoll(gb);

    // auto-roll for the ai
    if (gb->activePlayer == gb->aiPlayer) {
        SDL_AddTimer(500, timerAiRoll, gb);
    }
}
