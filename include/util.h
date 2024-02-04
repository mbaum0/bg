/**
 * @file util.h
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define WINDOW_W (430) 
#define WINDOW_H (932) 

#define Z_BACKGROUND 0 
#define Z_BOARD 1
#define Z_PIPS 2
#define Z_TRIM 3
#define Z_CHECKERS 4
#define Z_DICE 5
#define Z_BUTTONS 6

#define X(x) (Sage_convertHighDPI(x))

#define FELT_W X(430)
#define FELT_H X(932)

#define BOARD_W X(430)
#define BOARD_H X(700)
#define BOARD_X X(0)
#define BOARD_Y X(116)

#define PIP_W X(34)
#define PIP_H X(160)

#define PIP_6_X X(221)
#define PIP_6_Y X(490) 
#define PIP_12_X X(5)
#define PIP_12_Y X(490)
#define PIP_13_X X(5)
#define PIP_13_Y X(50)
#define PIP_19_X X(221) 
#define PIP_19_Y X(50) 

#define CHECKER_W X(30)
#define CHECKER_PIP_6_X X(223)
#define CHECKER_PIP_6_Y X(620) 
#define CHECKER_PIP_12_X X(7)
#define CHECKER_PIP_12_Y X(620) 
#define CHECKER_PIP_13_X X(7)
#define CHECKER_PIP_13_Y X(50)
#define CHECKER_PIP_19_X X(223) 
#define CHECKER_PIP_19_Y X(50)

#define CHECKER_TOP_HOME_X X(5)
#define CHECKER_TOP_HOME_Y X(10)
#define CHECKER_BOT_HOME_X X(5)
#define CHECKER_BOT_HOME_Y X(660)
#define CHECKER_BAR_X X(200)
#define CHECKER_BAR_TOP_Y X(431)
#define CHECKER_BAR_BOT_Y X(471)

#define DICE_W X(35)
#define DICE_LEFT_X X(67)
#define DICE_RIGHT_X X(284)
#define DICE_GAP X(45)
#define DICE_Y X(448)

#define BTN_W DICE_W
#define BTN_LEFT_X DICE_LEFT_X
#define BTN_RIGHT_X DICE_RIGHT_X
#define BTN_GAP DICE_GAP
#define BTN_Y DICE_Y

#define CHECKER_VELOCITY X(20)
#define DICE_VELOCITY X(5)

#define EPSILON 0.0001

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

bool isBetween(int32_t x, int32_t min, int32_t max);
bool isEqual(float x, float y, float epsilon);
int generateRandomNumber(int min, int max);
