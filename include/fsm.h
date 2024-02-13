#pragma once
#include "checker.h"
#include "dice.h"
#include "buttons.h"
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
  Color activePlayer;
};

typedef enum {
    CONFIRMED_MOVE_EVENT,
    UNDO_MOVE_EVENT,
    DICE_ROLL_EVENT,
    DICE_CLICKED_EVENT,
    PIP_SELECTED_EVENT
} Event;

typedef struct {
    Event event_type;
} EventData;

typedef enum {
    WAIT_FOR_ROLL_STATE,
    PLAYER_MOVE_STATE,
    MOVE_CONFIRM_STATE,
    GAME_OVER_STATE,
    NUM_STATES
} State;
// Define event queue
#define MAX_EVENTS 10
typedef struct {
    EventData events[MAX_EVENTS];
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
void fsm_enqueue_event(Event event);
bool fsm_dequeue_event(EventData* event);
void fsm_step(void);
void fsm_transition(State next_state);

void wait_for_roll_state(void);
void wait_for_roll_init_state(void);
void player_move_state(void);
void player_move_init_state(void);
void move_confirm_state(void);
void move_confirm_init_state(void);
void game_over_state(void);
void game_over_init_state(void);
