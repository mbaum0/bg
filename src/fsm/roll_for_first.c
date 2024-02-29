/**
 * @file roll_for_first.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief 
 */
#include "fsm.h"
#include "game.h"

Uint32 timerRollDice(Uint32 interval, void* ctx){
    (void)interval;
    (void)ctx;
    FSMEvent e = {ROLL_DICE_EVENT, 0, NULL};
    fsm_enqueue_event(e);
    return 0;
}

Uint32 timerGetFirstPlayer(Uint32 interval, void* ctx){
    (void)interval;
    GameBoard* gb = (GameBoard*)ctx;

    Color firstPlayer = NONE; // default to NONE, which is a tie
    if (gb->die1.value > gb->die2.value){
        firstPlayer = DARK;
    } else if (gb->die2.value > gb->die1.value){
        firstPlayer = LIGHT;
    }

    FSMEvent e = {ROLLED_FOR_FIRST_EVENT, firstPlayer, NULL};
    fsm_enqueue_event(e);
    return 0;
}

Uint32 timerTransitionToPlayerState(Uint32 interval, void* ctx){
    (void)interval;
    (void)ctx;
    FSMEvent e = {GOT_FIRST_PLAYER_EVENT, 0, NULL};
    fsm_enqueue_event(e);
    return 0;
}


void roll_for_first_state(FiniteStateMachine* fsm){
    GameBoard* gb = &fsm->gb;
    (void)gb;
    FSMEvent event;
    while (fsm_dequeue_event(&event)) {
        if (event.etype == ROLL_DICE_EVENT){
            rollDice(gb);
            SDL_AddTimer(500, timerGetFirstPlayer, gb);
        }

        if (event.etype == ROLLED_FOR_FIRST_EVENT){
            if (event.code == LIGHT){
                gb->activePlayer = LIGHT;
                gb->die1.side = 1;
                gb->die2.side = 1;
                SDL_AddTimer(400, timerTransitionToPlayerState, NULL);
            } else if (event.code == DARK){
                gb->activePlayer = DARK;
                gb->die1.side = 0;
                gb->die2.side = 0;
                SDL_AddTimer(400, timerTransitionToPlayerState, NULL);
            } else {
                // do another roll
                SDL_AddTimer(500, timerRollDice, gb);
                continue;
            }
        }

        if (event.etype == GOT_FIRST_PLAYER_EVENT){
            fsm_transition(PLAYER_MOVE_STATE);
        }
    }
}
void roll_for_first_init_state(FiniteStateMachine* fsm){
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: ROLL_FOR_FIRST");
    initBoardForRollForFirst(gb);

    // roll the dice
    SDL_AddTimer(500, timerRollDice, gb);
}
