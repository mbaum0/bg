#include "fsm.h"
#include <stdio.h>
void game_over_state(FiniteStateMachine* fsm, void* ctx) {
    GameBoard* gameboard = (GameBoard*)ctx;
}
void game_over_init_state(FiniteStateMachine* fsm, void* ctx) {
    GameBoard* gameboard = (GameBoard*)ctx;
    // this is called when the game_over state is first entered
}