#pragma once

#include "game.h"
#include <stdbool.h>

// typedef enum {
//     INIT_STATE,
//     ROLL_FOR_FIRST_STATE,
//     WAIT_FOR_ROLL_STATE,
//     PLAYER_MOVE_STATE,
//     MOVE_CONFIRM_STATE,
//     MATCH_OVER_STATE,
//     NUM_STATES
// } State;

#define FOREACH_STATE(STATE)                                                                                           \
    STATE(INIT_STATE)                                                                                                  \
    STATE(ROLL_FOR_FIRST_STATE)                                                                                        \
    STATE(WAIT_FOR_ROLL_STATE)                                                                                         \
    STATE(PLAYER_MOVE_STATE)                                                                                           \
    STATE(MOVE_CONFIRM_STATE)                                                                                          \
    STATE(MATCH_OVER_STATE)

typedef enum {
#define GENERATE_ENUM(ENUM) ENUM,
    FOREACH_STATE(GENERATE_ENUM)
#undef GENERATE_ENUM
        NUM_STATES
} State;

#define FOREACH_EVENT(EVENT)                                                                                           \
    EVENT(ENTERED_INIT_STATE_EVENT)                                                                                    \
    EVENT(ENTERED_ROLL_FOR_FIRST_STATE_EVENT)                                                                          \
    EVENT(ENTERED_WAIT_FOR_ROLL_STATE_EVENT)                                                                           \
    EVENT(ENTERED_PLAYER_MOVE_STATE_EVENT)                                                                             \
    EVENT(ENTERED_MOVE_CONFIRM_STATE_EVENT)                                                                            \
    EVENT(ENTERED_MATCH_OVER_STATE_EVENT)                                                                              \
    /** Button-related events */                                                                                       \
    EVENT(ROLL_BUTTON_CLICKED_EVENT)                                                                                   \
    EVENT(UNDO_BUTTON_CLICKED_EVENT)                                                                                   \
    EVENT(PIP_CLICKED_EVENT)                                                                                           \
    EVENT(DICE_CLICKED_EVENT)                                                                                          \
    EVENT(CONFIRM_BUTTON_CLICKED_EVENT)                                                                                \
    /** Events for roll_for_first state */                                                                             \
    EVENT(ROLLED_FOR_FIRST_EVENT)                                                                                      \
    EVENT(GOT_FIRST_PLAYER_EVENT)                                                                                      \
    EVENT(TIED_ROLL_EVENT)                                                                                             \
    EVENT(FINISHED_ROLL_FOR_FIRST_EVENT)                                                                               \
    /** Events for the wait_for_roll state */                                                                          \
    EVENT(DICE_ROLLED_EVENT)                                                                                           \
    EVENT(AI_DELAYED_DICE_ROLL_EVENT)                                                                                  \
    EVENT(FINISH_WAIT_FOR_ROLL_EVENT)                                                                                  \
    /** Events for player_move state */                                                                                \
    EVENT(AI_MOVE_EVENT)                                                                                               \
    EVENT(AI_SWAP_DICE_EVENT)                                                                                          \
    EVENT(FINISHED_PLAYER_MOVE_EVENT)

typedef enum {
#define GENERATE_ENUM(ENUM) ENUM,
    FOREACH_EVENT(GENERATE_ENUM)
#undef GENERATE_ENUM
        NUM_EVENTS
} FSMEventType;

typedef struct {
    FSMEventType etype;
    Sint32 code;
    void* ctx;
} FSMEvent;

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
} FiniteStateMachine;

void fsm_init(void);
void fsm_enqueue_event(FSMEvent event);
void fsm_enqueue_event_delay(Uint32 delay, FSMEvent e);
bool fsm_dequeue_event(FSMEvent* event);
void fsm_step(void);

// always return immediately after calling
void fsm_transition(State next_state);

void init_state(FiniteStateMachine* fsm);
void roll_for_first_state(FiniteStateMachine* fsm);
void wait_for_roll_state(FiniteStateMachine* fsm);
void player_move_state(FiniteStateMachine* fsm);
void move_confirm_state(FiniteStateMachine* fsm);
void match_over_state(FiniteStateMachine* fsm);

void save_state(FiniteStateMachine* fsm);
void load_state(FiniteStateMachine* fsm);
