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

// #define BOARD_SRC_W 1290
// #define BOARD_SRC_H 2195
// #define FELT_SRC_W 1290
// #define FELT_SRC_H 2796
#define FELT_SRC_H 932
#define FELT_SRC_W 430

#define GAME_W FELT_SRC_W
#define GAME_H FELT_SRC_H


#define EPSILON 0.0001

bool isBetween(int32_t x, int32_t min, int32_t max);
bool isEqual(float x, float y, float epsilon);
