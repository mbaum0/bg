/**
 * @file util.h
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define Z_BACKGROUND 0 
#define Z_BOARD 1
#define Z_PIPS 2
#define Z_TRIM 3
#define Z_CHECKERS 4

#define BOARD_SRC_W 1630
#define BOARD_SRC_H 1260
#define TRIM_SRC_W 3408
#define TRIM_SRC_H 2668
#define PIP_SRC_W 62
#define PIP_SRC_H 310
#define PIP_RATIO 5

#define BOARD_X_NORMAL 0.021713615023474
#define BOARD_Y_NORMAL 0.027736131934033
#define BOARD_W_NORMAL 0.956572769953052
#define BOARD_H_NORMAL 0.944527736131934

#define TRIM_X_NORMAL .0
#define TRIM_Y_NORMAL .0
#define TRIM_H_NORMAL 1.0
#define TRIM_W_NORMAL 1.0

#define PIP_RIGHT_X_OFFSET_NORMAL 0.526408450704225
#define PIP_LEFT_X_OFFSET_NORMAL 0.121478873239437
#define PIP_TOP_Y_OFFSET_NORMAL 0.050224887556222
#define PIP_BOTTOM_Y_OFFSET_NORMAL 0.050224887556222
#define PIP_WIDTH_NORMAL 0.05868544600939
#define PIP_HEIGHT_NORMAL 0.374812593703148

bool isBetween(int32_t x, int32_t min, int32_t max);
