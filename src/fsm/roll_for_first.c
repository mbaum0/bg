/**
 * @file roll_for_first.c
 * @author Michael Baumgarten (you@domain.com)
 * @brief
 */
#include "fsm.h"
#include "game.h"

Player getFirstPlayer(GameBoard* gb) {
    if (gb->die1.value > gb->die2.value) {
        return PLAYER_TWO;
    } else if (gb->die2.value > gb->die1.value) {
        return PLAYER_ONE;
    }
    return PLAYER_NONE;
}

bool handleEnteredRollForFirstEvent(GameBoard* gb) {
    initBoardForRollForFirst(gb);

    rollDice(gb);
    FSMEvent e = {ROLLED_FOR_FIRST_EVENT, 0, NULL};
    fsm_enqueue_event_delay(500, e);
    return false;
}

bool handleRollForFirstEvent(GameBoard* gb) {
    Player fp = getFirstPlayer(gb);
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
    Player fp = getFirstPlayer(gb);
    if (fp == PLAYER_ONE) {
        gb->activePlayer = PLAYER_ONE;
        gb->die1.side = 1;
        gb->die2.side = 1;
    } else {
        gb->activePlayer = PLAYER_TWO;
        gb->die1.side = 0;
        gb->die2.side = 0;
    }
    FSMEvent e = {FINISHED_ROLL_FOR_FIRST_EVENT, 0, NULL};
    fsm_enqueue_event_delay(500, e);
    return false;
}

bool handleFinishRollForFirstEvent(GameBoard* gb) {
    (void)gb;
    fsm_transition(PLAYER_MOVE_STATE);
    return true;
}

void roll_for_first_state(FiniteStateMachine* fsm) {
    GameBoard* gb = &fsm->gb;
    (void)gb;
    FSMEvent event;
    bool quit = false;
    while (!quit && fsm_dequeue_event(&event)) {

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
        case FINISHED_ROLL_FOR_FIRST_EVENT:
            quit = handleFinishRollForFirstEvent(gb);
            break;
        default:
            break;
        }
    }
}
