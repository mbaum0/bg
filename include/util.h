/**
 * @file util.h
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#pragma once
#include <stdbool.h>
#include <stdint.h>

#define WINDOW_W (430)
#define WINDOW_H (932)

#define Z_BACKGROUND 0
#define Z_BOARD 1
#define Z_PIPS 2
#define Z_CHECKERS 3
#define Z_TRIM 4
#define Z_BAR_CHECKERS 5
#define Z_DICE 6
#define Z_BUTTONS 7
#define Z_DEBUG 8

#define X(x) (Sage_convertHighDPI(x))

#define FELT_W X(430)
#define FELT_H X(932)

#define TRIM_W X(430)
#define TRIM_H X(733)
#define TRIM_X X(0)
#define TRIM_Y X(99)

#define BOARD_W X(430)
#define BOARD_H X(700)
#define BOARD_X X(0)
#define BOARD_Y X(17)

#define PIP_W X(34)
#define PIP_H X(160)

#define PIP_6_X X(221)
#define PIP_6_Y X(507)
#define PIP_12_X X(5)
#define PIP_12_Y X(507)
#define PIP_13_X X(5)
#define PIP_13_Y X(66.562)
#define PIP_19_X X(221)
#define PIP_19_Y X(66.562)

#define CHECKER_W X(36)
#define CHECKER_PIP_6_X X(220)
#define CHECKER_PIP_6_Y X(635.3)
#define CHECKER_PIP_12_X X(4)
#define CHECKER_PIP_12_Y X(635.3)
#define CHECKER_PIP_13_X X(4)
#define CHECKER_PIP_13_Y X(62.5)
#define CHECKER_PIP_19_X X(220)
#define CHECKER_PIP_19_Y X(62.5)

#define CHECKER_TOP_HOME_X X(2.7)
#define CHECKER_TOP_HOME_Y X(24.4)
#define CHECKER_BOT_HOME_X X(3.3)
#define CHECKER_BOT_HOME_Y X(674)
#define CHECKER_BAR_X X(197.2)
#define CHECKER_BAR_TOP_Y X(290.5)
#define CHECKER_BAR_BOT_Y X(407.4)

#define DICE_W X(45)
#define DICE_LEFT_X X(58.8)
#define DICE_RIGHT_X X(279.8)
#define DICE_GAP X(47)
#define DICE_Y X(344.5)

#define BTN_W DICE_W
#define BTN_LEFT_X DICE_LEFT_X
#define BTN_RIGHT_X DICE_RIGHT_X
#define BTN_GAP DICE_GAP
#define BTN_Y DICE_Y

#define CHECKER_VELOCITY X(20)
#define DICE_VELOCITY X(5)

#define DEBUG_FONT_SIZE X(20)
#define DEBUG_X X(2)
#define DEBUG_Y BOARD_Y + BOARD_H

#define EPSILON 0.0001

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

bool isBetween(int32_t x, int32_t min, int32_t max);
bool isEqual(float x, float y, float epsilon);
int generateRandomNumber(int min, int max);
