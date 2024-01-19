/**
 * @file board.h
 * @author Michael Baumgarten
 * @brief board and trim objects
 */
#pragma once
#include "sage.h"
#include <stdint.h>

#define BOARD_SRC_W 3140
#define BOARD_SRC_H 2400
#define TRIM_SRC_W 3408
#define TRIM_SRC_H 2668
#define PIP_SRC_W 62
#define PIP_SRC_H 310
#define PIP_RATIO 5

#define PIP_BOARD_X_OFFSET_PCT .08917
#define PIP_BOARD_WIDTH_PCT .06369

#define TRIM_BOARD_X_OFFSET_PCT .04268
#define TRIM_BOARD_Y_OFFSET_PCT .05583
#define TRIM_BOARD_WIDTH_PCT 1.08535
#define TRIM_BOARD_HEIGHT_PCT 1.11167

#define BOARD_W_PERCENT .5
#define BOARD_H_PERCENT .75

#define MAX_ASPECT_RATIO 1.4
#define MIN_ASPECT_RATIO .68

void Board_create(Sage* sage);
