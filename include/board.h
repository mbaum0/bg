/**
 * @file board.h
 * @author Michael Baumgarten
 * @brief Defines the GameBoard, Checkers, and other game related structs
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct Player Player;
typedef struct Checker Checker;
typedef struct Dice Dice;
typedef struct GameBoard GameBoard;

#define LOC_HOME_LIGHT 0
#define LOC_HOME_DARK 25
#define LOC_BAR_LIGHT LOC_HOME_DARK
#define LOC_BAR_DARK LOC_HOME_LIGHT

typedef enum {
    P_Dark,
    P_Light,
    P_None
} Player;

struct Checker {
    Player player; // player that owns the checker
    uint32_t location; // 0 - light's home, 1 - 24 - pips, 25 - dark's home
    uint32_t index; // the index of the checker at a given location (between 0 and 4 if on a pip)
};

struct Dice {
    uint32_t die_0;
    uint32_t die_1;
};

struct GameBoard {
    Checker checkers[30];
    Dice dice;
};

/**
 * @brief Return the number of checkers at a given location
 */
uint32_t Board_getNumCheckersAtLocation(GameBoard* board, uint32_t location);

/**
 * @brief Return the owner of a given pip
 */
Player Board_getPipOwner(GameBoard* board, uint32_t pip);

/**
 * @brief Returns true if a checker can move to a given location
 */
bool Board_canMoveChecker(GameBoard* board, Checker* checker, uint32_t toLocation);

/**
 * @brief Move a checker to a given location
 */
void Board_moveChecker(GameBoard* board, Checker* checker, uint32_t location);

/**
 * @brief Return the number of checkers a player has on the bar
 */
uint32_t Board_getNumCheckersOnBar(GameBoard* board, Player player);

/**
 * @brief Roll the dice
 */
void Board_rollDice(GameBoard* board);

/**
 * @brief Swap the dice
 */
void Board_swapDice(GameBoard* board);

/**
 * @brief Initialize the game board to a new game state. Must be freed with Board_free
 * 
 * @return GameBoard* 
 */
GameBoard* Board_init(void);

/**
 * @brief Free memory allocated by the game board
 */
void Board_free(GameBoard* board);
