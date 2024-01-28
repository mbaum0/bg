/**
 * @file util.h
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define MULTIPLYER (3)
#define WINDOW_W (430) 
#define WINDOW_H (932) 

#define Z_BACKGROUND 0 
#define Z_BOARD 1
#define Z_PIPS 2
#define Z_TRIM 3
#define Z_CHECKERS 4
#define Z_DICE 5

#define FELT_W 430 
#define FELT_H 932 

#define BOARD_W 430 
#define BOARD_H 700 
#define BOARD_X 0
#define BOARD_Y 116 

#define PIP_W 34 
#define PIP_H 160 

#define PIP_6_X 221 
#define PIP_6_Y 490 
#define PIP_12_X 5 
#define PIP_12_Y 490 
#define PIP_13_X 5 
#define PIP_13_Y 50 
#define PIP_19_X 221 
#define PIP_19_Y 50 

#define CHECKER_W 30 
#define CHECKER_PIP_6_X 223 
#define CHECKER_PIP_6_Y 620 
#define CHECKER_PIP_12_X 7 
#define CHECKER_PIP_12_Y 620 
#define CHECKER_PIP_13_X 7 
#define CHECKER_PIP_13_Y 50 
#define CHECKER_PIP_19_X 223 
#define CHECKER_PIP_19_Y 50 

#define CHECKER_VELOCITY 5 

#define EPSILON 0.0001

bool isBetween(int32_t x, int32_t min, int32_t max);
bool isEqual(float x, float y, float epsilon);
