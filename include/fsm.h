#pragma once

#include "game.h"
#include <stdbool.h>

typedef enum {
    CONFIRMED_MOVE_EVENT,
    UNDO_MOVE_EVENT,
    ROLL_DICE_EVENT,
    AI_ROLL_DICE_EVENT,
    DICE_CLICKED_EVENT,
    PIP_CLICKED_EVENT,
    ROLLED_FOR_FIRST_EVENT,
    GOT_FIRST_PLAYER_EVENT,
    PLAYER_HAS_NO_MOVES_EVENT,
    AI_START_TURN_EVENT,
    AI_END_TURN_EVENT,
    AI_SWAP_DICE_EVENT,
    AI_MOVE_EVENT,
    SHOW_NO_MOVES_ICON_EVENT,
} FSMEventType;

typedef struct {
    FSMEventType etype;
    Sint32 code;
    void* ctx;
} FSMEvent;

typedef enum {
    INIT_STATE,
    ROLL_FOR_FIRST_STATE,
    WAIT_FOR_ROLL_STATE,
    PLAYER_MOVE_STATE,
    MOVE_CONFIRM_STATE,
    MATCH_OVER_STATE,
    NUM_STATES
} State;

// Define event queue
#define MAX_EVENTS 10
typedef struct {
    FSMEvent events[MAX_EVENTS];
    int front, rear;
} EventQueue;

typedef struct FiniteStateMachine {
    State current_state;
    EventQueue eventQueue;
    GameBoard gb;
    void (*state_functions[NUM_STATES])(struct FiniteStateMachine*);
    void (*state_init_functions[NUM_STATES])(struct FiniteStateMachine*);
} FiniteStateMachine;

void fsm_init(void);
void fsm_enqueue_event(FSMEvent event);
void fsm_enqueue_event_delay(Uint32 delay, FSMEvent e);
bool fsm_dequeue_event(FSMEvent* event);
void fsm_step(void);

// always return immediately after calling
void fsm_transition(State next_state);

void init_state(FiniteStateMachine* fsm);
void init_init_state(FiniteStateMachine* fsm);
void roll_for_first_state(FiniteStateMachine* fsm);
void roll_for_first_init_state(FiniteStateMachine* fsm);
void wait_for_roll_state(FiniteStateMachine* fsm);
void wait_for_roll_init_state(FiniteStateMachine* fsm);
void player_move_state(FiniteStateMachine* fsm);
void player_move_init_state(FiniteStateMachine* fsm);
void move_confirm_state(FiniteStateMachine* fsm);
void move_confirm_init_state(FiniteStateMachine* fsm);
void match_over_state(FiniteStateMachine* fsm);
void match_over_init_state(FiniteStateMachine* fsm);

void save_state(FiniteStateMachine* fsm);
void load_state(FiniteStateMachine* fsm);
