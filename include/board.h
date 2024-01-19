/**
 * @file board.h
 * @author Michael Baumgarten
 * @brief board and trim objects
 */
#pragma once
#include "sage.h"
#include <stdint.h>

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

// #define MAX_ASPECT_RATIO 1.4
// #define MIN_ASPECT_RATIO .68

void Board_create(Sage* sage);
