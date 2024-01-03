/**
 * @file view.h
 * @author Michael Baumgarten
 * @brief ViewManager is responsible for:
 *  1. Managing sprites
 *  2. Rendering sprites to the screen
 *  3. Passing user input events to the controller
 */
#pragma once
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "events.h"
#include <SDL_ttf.h>

#define BOARD_HEIGHT 1080
#define BOARD_WIDTH 1560
#define PLAY_AREA_WIDTH 540
#define PLAY_AREA_HEIGHT 1000
#define CENTER_GAP_WIDTH 90
#define PIP_WIDTH 90
#define CHECKER_SIZE 80
#define PIP_HEIGHT (PIP_WIDTH * 5)

#define WINDOW_SIDE_OFFSET ((WINDOW_WIDTH - BOARD_WIDTH) / 2)
#define WINDOW_TOP_OFFSET ((WINDOW_HEIGHT - BOARD_HEIGHT) / 2)
#define PLAY_AREA_TOP_OFFSET (WINDOW_TOP_OFFSET + 40)

#define PIP_CHECKER_OFFSET ((PIP_WIDTH - CHECKER_SIZE) / 2)
#define PLAY_AREA_LEFT_SIDE_X_OFFSET (WINDOW_SIDE_OFFSET + 195)
#define PLAY_AREA_RIGHT_SIDE_X_OFFSET (PLAY_AREA_LEFT_SIDE_X_OFFSET + CENTER_GAP_WIDTH + (6 * PIP_WIDTH))

#define GET_CHECKER_X(pip) \
    ((pip < 1 || pip > 24) ? 0 : \
        ((pip >= 1 && pip <= 6) ? (PLAY_AREA_RIGHT_SIDE_X_OFFSET + PIP_CHECKER_OFFSET + ((6 - pip) * PIP_WIDTH)) : \
        ((pip >= 7 && pip <= 12) ? (PLAY_AREA_LEFT_SIDE_X_OFFSET + PIP_CHECKER_OFFSET + ((12 - pip) * PIP_WIDTH)) : \
        ((pip >= 13 && pip <= 18) ? (PLAY_AREA_LEFT_SIDE_X_OFFSET + PIP_CHECKER_OFFSET + ((pip - 13) * PIP_WIDTH)) : \
        (PLAY_AREA_RIGHT_SIDE_X_OFFSET + PIP_CHECKER_OFFSET + ((pip - 19) * PIP_WIDTH))))))
#define GET_CHECKER_Y(pip, index) \
    ((pip < 1 || pip > 24) ? 0 : \
    ((pip >= 1 && pip <= 12) ? (PLAY_AREA_TOP_OFFSET + BOARD_HEIGHT - ((2 +  index) * CHECKER_SIZE)) : \
    (PLAY_AREA_TOP_OFFSET + ((index) * CHECKER_SIZE))))

#define GET_PIP_X(pip) \
    ((pip >= 0 && pip <= 5) ? (PLAY_AREA_RIGHT_SIDE_X_OFFSET + ((5 - pip) * PIP_WIDTH)) : \
    ((pip >= 6 && pip <= 11) ? (PLAY_AREA_LEFT_SIDE_X_OFFSET + ((11 - pip) * PIP_WIDTH)) : \
    ((pip >= 12 && pip <= 17) ? (PLAY_AREA_LEFT_SIDE_X_OFFSET + ((pip - 12) * PIP_WIDTH)) : \
    (PLAY_AREA_RIGHT_SIDE_X_OFFSET + ((pip - 18) * PIP_WIDTH)))))

#define GET_PIP_Y(pip) \
    ((pip >= 0 && pip <= 11) ? (PLAY_AREA_TOP_OFFSET + BOARD_HEIGHT - PIP_HEIGHT - CHECKER_SIZE) : (PLAY_AREA_TOP_OFFSET))


#define MAX_TEXT_LENGTH 100
#define Z_BOARD 1
#define Z_PIP 2
#define Z_DICE 3
#define Z_BUTTON 4
#define Z_CHECKER 5
#define Z_CHECKERTEXT 6
#define Z_DEBUGTEXT 7

