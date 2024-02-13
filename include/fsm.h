#pragma once
#include <stdbool.h>

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
    void (*state_functions[NUM_STATES])(struct FiniteStateMachine*, void*);
    void (*state_init_functions[NUM_STATES])(struct FiniteStateMachine*, void*);
} FiniteStateMachine;

void fsm_init(FiniteStateMachine* fsm);
void fsm_enqueue_event(FiniteStateMachine* fsm, Event event);
bool fsm_dequeue_event(FiniteStateMachine* fsm, EventData* event);
void fsm_step(FiniteStateMachine* fsm, void* ctx);
void fsm_transition(FiniteStateMachine *fsm, State next_state, void* ctx);

typedef struct {} GameBoard;
void wait_for_roll_state(FiniteStateMachine* fsm, void* ctx);
void wait_for_roll_init_state(FiniteStateMachine* fsm, void* ctx);
void player_move_state(FiniteStateMachine* fsm, void* ctx);
void player_move_init_state(FiniteStateMachine* fsm, void* ctx);
void move_confirm_state(FiniteStateMachine* fsm, void* ctx);
void move_confirm_init_state(FiniteStateMachine* fsm, void* ctx);
void game_over_state(FiniteStateMachine* fsm, void* ctx);
void game_over_init_state(FiniteStateMachine* fsm, void* ctx);
