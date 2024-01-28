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

#define CHECKER_VELOCITY X(5)

#define EPSILON 0.0001

bool isBetween(int32_t x, int32_t min, int32_t max);
bool isEqual(float x, float y, float epsilon);
