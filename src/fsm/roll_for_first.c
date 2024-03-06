/**
 * @file roll_for_first.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "fsm.h"
#include "game.h"

// Uint32 timerRollDice(Uint32 interval, void* ctx) {
//     (void)interval;
//     (void)ctx;
//     FSMEvent e = {ROLL_DICE_EVENT, 0, NULL};
//     fsm_enqueue_event(e);
//     return 0;
// }

// Uint32 timerGetFirstPlayer(Uint32 interval, void* ctx) {
//     (void)interval;
//     GameBoard* gb = (GameBoard*)ctx;

//     Color firstPlayer = NONE; // default to NONE, which is a tie
//     if (gb->die1.value > gb->die2.value) {
//         firstPlayer = DARK;
//     } else if (gb->die2.value > gb->die1.value) {
//         firstPlayer = LIGHT;
//     }

//     FSMEvent e = {ROLLED_FOR_FIRST_EVENT, firstPlayer, NULL};
//     fsm_enqueue_event(e);
//     return 0;
// }

// Uint32 timerTransitionToPlayerState(Uint32 interval, void* ctx) {
//     (void)interval;
//     (void)ctx;
//     FSMEvent e = {GOT_FIRST_PLAYER_EVENT, 0, NULL};
//     fsm_enqueue_event(e);
//     return 0;
// }

Color getFirstPlayer(GameBoard* gb) {
    if (gb->die1.value > gb->die2.value) {
        return DARK;
    } else if (gb->die2.value > gb->die1.value) {
        return LIGHT;
    }
    return NONE;
}

bool handleEnteredRollForFirstEvent(GameBoard* gb) {
    rollDice(gb);
    FSMEvent e = {ROLLED_FOR_FIRST_EVENT, 0, NULL};
    fsm_enqueue_event_delay(500, e);
    return false;
}

bool handleRollForFirstEvent(GameBoard* gb) {
    Color fp = getFirstPlayer(gb);
    if (fp == NONE) {
        FSMEvent e = {TIED_ROLL_EVENT, 0, NULL};
        fsm_enqueue_event_delay(500, e);
    } else {
        FSMEvent e = {GOT_FIRST_PLAYER_EVENT, 0, NULL};
        fsm_enqueue_event_delay(500, e);
    }
    return false;
}

bool handleTiedRollEvent(GameBoard* gb) {
    rollDice(gb);
    FSMEvent e = {ROLLED_FOR_FIRST_EVENT, 0, NULL};
    fsm_enqueue_event_delay(500, e);
    return false;
}

bool handleGotFirstPlayerEvent(GameBoard* gb) {
    Color fp = getFirstPlayer(gb);
    if (fp == LIGHT) {
        gb->activePlayer = LIGHT;
        gb->die1.side = 1;
        gb->die2.side = 1;
    } else {
        gb->activePlayer = DARK;
        gb->die1.side = 0;
        gb->die2.side = 0;
    }
    FSMEvent e = {FINISH_ROLL_FOR_FIRST_EVENT, 0, NULL};
    fsm_enqueue_event_delay(500, e);
    return false;
}

bool handleFinishRollForFirstEvent(GameBoard* gb){
    (void)gb;
    fsm_transition(PLAYER_MOVE_STATE);
    return true;
}

void roll_for_first_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    (void)gb;
    FSMEvent event;
    bool quit = false;
    while (fsm_dequeue_event(&event) && !quit) {

        switch (event.etype) {
        case ENTERED_ROLL_FOR_FIRST_STATE_EVENT:
            quit = handleEnteredRollForFirstEvent(gb);
            break;
        case ROLLED_FOR_FIRST_EVENT:
            quit = handleRollForFirstEvent(gb);
            break;
        case TIED_ROLL_EVENT:
            quit = handleTiedRollEvent(gb);
            break;
        case GOT_FIRST_PLAYER_EVENT:
            quit = handleGotFirstPlayerEvent(gb);
            break;
        case FINISH_ROLL_FOR_FIRST_EVENT:
            quit = handleFinishRollForFirstEvent(gb);
            break;
        default:
            break;
        }

        // if (event.etype == ROLL_DICE_EVENT) {
        //     rollDice(gb);
        //     FSMEvent e = {GET_FIRST_PLAYER_EVENT, 0, NULL};
        //     fsm_enqueue_event_delay(500, e);
        // }

        // if (event.etype == GET_FIRST_PLAYER_EVENT) {
        //     Color firstPlayer = NONE; // default to NONE, which is a tie
        //     if (gb->die1.value > gb->die2.value) {
        //         firstPlayer = DARK;
        //     } else if (gb->die2.value > gb->die1.value) {
        //         firstPlayer = LIGHT;
        //     }

        //     FSMEvent e = {ROLLED_FOR_FIRST_EVENT, firstPlayer, NULL};
        //     fsm_enqueue_event(e);
        // }

        // if (event.etype == ROLLED_FOR_FIRST_EVENT) {
        //     if (event.code == LIGHT) {
        //         gb->activePlayer = LIGHT;
        //         gb->die1.side = 1;
        //         gb->die2.side = 1;
        //         FSMEvent e = {GOT_FIRST_PLAYER_EVENT, 0, NULL};
        //         fsm_enqueue_event_delay(400, e);
        //     } else if (event.code == DARK) {
        //         gb->activePlayer = DARK;
        //         gb->die1.side = 0;
        //         gb->die2.side = 0;
        //         FSMEvent e = {GOT_FIRST_PLAYER_EVENT, 0, NULL};
        //         fsm_enqueue_event_delay(400, e);
        //     } else {
        //         // do another roll
        //         FSMEvent e = {ROLL_DICE_EVENT, 0, NULL};
        //         fsm_enqueue_event_delay(500, e);
        //         continue;
        //     }
        // }

        // if (event.etype == GOT_FIRST_PLAYER_EVENT) {
        //     fsm_transition(PLAYER_MOVE_STATE);
        //     break;
        // }
    }
}
void roll_for_first_init_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    log_debug("Entered state: ROLL_FOR_FIRST");
    initBoardForRollForFirst(gb);

    // roll the dice
    FSMEvent e = {ENTERED_ROLL_FOR_FIRST_STATE_EVENT, 0, NULL};
    fsm_enqueue_event_delay(500, e);
}
