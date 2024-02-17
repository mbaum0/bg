#pragma once
#include "buttons.h"
#include "checker.h"
#include "dice.h"
#include <stdbool.h>

typedef struct GameBoard GameBoard;

struct GameBoard {
    Checker lightCheckers[15];
    Checker lightCheckersSave[15];
    Checker darkCheckers[15];
    Checker darkCheckersSave[15];
    GameDie die1;
    GameDie die2;
    GameButton confirm;
    GameButton undo;
    GameButton roll;
    Color activePlayer;
    Color aiPlayer;
};

typedef enum {
    CONFIRMED_MOVE_EVENT,
    UNDO_MOVE_EVENT,
    ROLL_DICE_EVENT,
    AI_ROLL_DICE_EVENT,
    DICE_CLICKED_EVENT,
    PIP_CLICKED_EVENT,
    AI_PIP_CLICKED_EVENT
} FSMEventType;

typedef struct {
    FSMEventType etype;
    int32_t code;
    void* ctx;
} FSMEvent;

typedef enum { INIT_STATE, WAIT_FOR_ROLL_STATE, PLAYER_MOVE_STATE, MOVE_CONFIRM_STATE, GAME_OVER_STATE, NUM_STATES } State;

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
bool fsm_dequeue_event(FSMEvent* event);
void fsm_step(void);
void fsm_transition(State next_state);

void init_state(FiniteStateMachine* fsm);
void init_init_state(FiniteStateMachine* fsm);
void wait_for_roll_state(FiniteStateMachine* fsm);
void wait_for_roll_init_state(FiniteStateMachine* fsm);
void player_move_state(FiniteStateMachine* fsm);
void player_move_init_state(FiniteStateMachine* fsm);
void move_confirm_state(FiniteStateMachine* fsm);
void move_confirm_init_state(FiniteStateMachine* fsm);
void game_over_state(FiniteStateMachine* fsm);
void game_over_init_state(FiniteStateMachine* fsm);
