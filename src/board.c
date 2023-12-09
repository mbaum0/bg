/**
 * @file board.c
 * @author Michael Baumgarten
 * @brief Defines the GameBoard and other game related structs
 */

#include "board.h"

GameBoard* Board_init(void) {
    GameBoard* board = malloc(sizeof(GameBoard));
    // Initialize the checkers
    for (int32_t i = 0; i < 30; i++) {
        board->checkers[i].player = P_None;
        board->checkers[i].location = i;
        board->checkers[i].index = 0;
    }
    // Initialize the dice
    board->dice.die_0 = 1;
    board->dice.die_1 = 1;

    board->clickedLocation = 0;
    board->clickedSprite = 0;


    int32_t checkerIndex = 0;
    // Set the initial checker locations
    for (int32_t i = 0; i < 2; i++) {
        board->checkers[checkerIndex].player = P_Dark;
        board->checkers[checkerIndex].location = 1;
        board->checkers[checkerIndex].index = i;
        checkerIndex++;

        board->checkers[checkerIndex].player = P_Light;
        board->checkers[checkerIndex].location = 24;
        board->checkers[checkerIndex].index = i;
        checkerIndex++;
    }

    for (int32_t i = 0; i< 5; i++){
        board->checkers[checkerIndex].player = P_Dark;
        board->checkers[checkerIndex].location = 12;
        board->checkers[checkerIndex].index = i;
        checkerIndex++;
        
        board->checkers[checkerIndex].player = P_Light;
        board->checkers[checkerIndex].location = 13;
        board->checkers[checkerIndex].index = i;
        checkerIndex++;
    }

    for (int32_t i = 0; i< 3; i++){
        board->checkers[checkerIndex].player = P_Dark;
        board->checkers[checkerIndex].location = 17;
        board->checkers[checkerIndex].index = i;
        checkerIndex++;

        board->checkers[checkerIndex].player = P_Light;
        board->checkers[checkerIndex].location = 8;
        board->checkers[checkerIndex].index = i;
        checkerIndex++;
    }

    for (int32_t i = 0; i< 5; i++){
        board->checkers[checkerIndex].player = P_Dark;
        board->checkers[checkerIndex].location = 19;
        board->checkers[checkerIndex].index = i;
        checkerIndex++;

        board->checkers[checkerIndex].player = P_Light;
        board->checkers[checkerIndex].location = 6;
        board->checkers[checkerIndex].index = i;
        checkerIndex++;
    }
    return board;
}

int32_t Board_getNumCheckersAtLocation(GameBoard* board, int32_t location) {
    int32_t numCheckers = 0;

    if (location == LOC_HOME_LIGHT || location == LOC_HOME_DARK) {
        return 0;
    }

    for (int32_t i = 0; i < 30; i++) {
        if (board->checkers[i].location == location) {
            numCheckers++;
        }
    }
    return numCheckers;
}

Player Board_getPipOwner(GameBoard* board, int32_t pip) {
    if (pip == 0) {
        return P_None;
    } else if (pip == 25) {
        return P_None;
    }
    
    int32_t numCheckers = Board_getNumCheckersAtLocation(board, pip);
    if (numCheckers == 0){
        return P_None;
    }

    Checker* checker = Board_getNextCheckerAtLocation(board, pip);

    return checker->player;
}

int32_t Board_getNumCheckersOnBar(GameBoard* board, Player player){
    int32_t numCheckers = 0;
    int32_t barLocation = (player == P_Dark) ? LOC_BAR_DARK : LOC_BAR_LIGHT;
    for (int32_t i = 0; i < 30; i++) {
        if (board->checkers[i].location == barLocation) {
            numCheckers++;
        }
    }
    return numCheckers;

}

bool Board_canMoveChecker(GameBoard* board, Checker* checker, int32_t toLocation) {
    Player owner = checker->player;
    int32_t barLocation = (owner == P_Dark) ? LOC_BAR_DARK : LOC_BAR_LIGHT;
    int32_t numLocationCheckers = Board_getNumCheckersAtLocation(board, toLocation);

    // Check if the checker is in the home location
    if ((checker->location == LOC_HOME_LIGHT && owner == P_Light) || (checker->location == LOC_HOME_DARK && owner == P_Dark)) {
        return false;
    }

    // Check if the checker is not on the bar, but there are checkers on the bar
    if (Board_getNumCheckersOnBar(board, owner) > 0 && checker->location != barLocation){
        return false;
    }

    // Check if toLocation is full
    if (numLocationCheckers == 5) {
        return false;
    }

    // Check if the toLocation is owner by another player and has more than 1 checker
    if (numLocationCheckers > 0 && Board_getPipOwner(board, toLocation) != owner) {
        return false;
    }

    return true;
}

void Board_moveChecker(GameBoard* board, Checker* checker, int32_t toLocation) {
    int32_t numLocationCheckers = Board_getNumCheckersAtLocation(board, toLocation);
    checker->index = numLocationCheckers;
    checker->location = toLocation;
}

Checker* Board_getNextCheckerAtLocation(GameBoard* board, int32_t location){
    int32_t highestIndex = 0;
    Checker* highestChecker = NULL;
    for (int32_t i = 0; i < 30; i++) {
        if (board->checkers[i].location == location) {
            if (board->checkers[i].index >= highestIndex){
                highestIndex = board->checkers[i].index;
                highestChecker = &board->checkers[i];
            }
        }
    }
    return highestChecker;
}

bool Board_moveIfPossible(GameBoard* board, int32_t fromLocation, int32_t amount){
    Checker* checker = Board_getNextCheckerAtLocation(board, fromLocation);
    if (checker == NULL){
        return false;
    }

    int32_t toLocation = (checker->player == P_Dark) ? fromLocation + amount : fromLocation - amount;
    if (Board_canMoveChecker(board, checker, toLocation)){
        Board_moveChecker(board, checker, toLocation);
        return true;
    }
    return false;
}

void Board_rollDice(GameBoard* board) {
    board->dice.die_0 = rand() % 6 + 1;
    board->dice.die_1 = rand() % 6 + 1;
}

void Board_swapDice(GameBoard* board) {
    int32_t temp = board->dice.die_0;
    board->dice.die_0 = board->dice.die_1;
    board->dice.die_1 = temp;
}

void Board_free(GameBoard* board) {
    free(board);
}
