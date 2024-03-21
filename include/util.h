/**
 * @file util.h
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#pragma once
#include <stdbool.h>

#include <SDL3/SDL.h>

#define WINDOW_W (430)
#define WINDOW_H (932)

#define Z_BACKGROUND 0
#define Z_BOARD 100
#define Z_PIPS 200
#define Z_CHECKERS 300
// #define Z_TRIM 400
// #define Z_BOARD_OVERLAY 500
#define Z_BAR_CHECKERS 600
#define Z_DICE 700
#define Z_BUTTONS 800
#define Z_MOVING_CHECKERS 900
#define Z_DIALOG_OVERLAY 1000
#define Z_DIALOG 1100
#define Z_DEBUG 1200

#define X(x) (Sage_convertHighDPI(x))

#define FELT_SRC_W (430)
#define FELT_SRC_H (932)
#define FELT_W X(FELT_SRC_W)
#define FELT_H X(FELT_SRC_H)

// #define TRIM_SRC_W (430)
// #define TRIM_SRC_H (733)
// #define TRIM_W X(TRIM_SRC_W)
// #define TRIM_H X(TRIM_SRC_H)
// #define TRIM_X X(0)
// #define TRIM_Y X(99)

#define BOARD_SRC_W (430)
#define BOARD_SRC_H (826)
#define BOARD_W X(BOARD_SRC_W)
#define BOARD_H X(BOARD_SRC_H)
#define BOARD_X X(0)
#define BOARD_Y X(60)

// #define BOARD_OVERLAY_SRC_W (403)
// #define BOARD_OVERLAY_SRC_H (618)
// #define BOARD_OVERLAY_W X(BOARD_OVERLAY_SRC_W)
// #define BOARD_OVERLAY_H X(BOARD_OVERLAY_SRC_H)
// #define BOARD_OVERLAY_X X(14)
// #define BOARD_OVERLAY_Y X(56.7)

#define PIP_SRC_W (34)
#define PIP_SRC_H (160)
#define PIP_W X(PIP_SRC_W)
#define PIP_H X(PIP_SRC_H)

#define PIP_6_X X(221)
#define PIP_6_Y X(567)
#define PIP_12_X X(5)
#define PIP_12_Y X(567)
#define PIP_13_X X(5)
#define PIP_13_Y X(147)
#define PIP_19_X X(221)
#define PIP_19_Y X(147)

#define CHECKER_SRC_W (28)
#define CHECKER_W X(CHECKER_SRC_W)
#define CHECKER_PIP_6_X X(224)
#define CHECKER_PIP_6_Y X(699)
#define CHECKER_PIP_12_X X(8)
#define CHECKER_PIP_12_Y X(699)
#define CHECKER_PIP_13_X X(8)
#define CHECKER_PIP_13_Y X(147)
#define CHECKER_PIP_19_X X(224)
#define CHECKER_PIP_19_Y X(147)

#define CHECKER_TOP_HOME_X X(20)
#define CHECKER_TOP_HOME_Y X(105)
#define CHECKER_BOT_HOME_X X(20)
#define CHECKER_BOT_HOME_Y X(742)

#define CHECKER_BAR_X X(201)
#define CHECKER_BAR_TOP_Y X(368)
#define CHECKER_BAR_BOT_Y X(476)
#define CHECKER_GAP X(26)

#define DICE_SRC_W (45)
#define DICE_W X(DICE_SRC_W)
#define DICE_LEFT_X X(60)
#define DICE_RIGHT_X X(276)
#define DICE_GAP X(50)
#define DICE_Y X(415)

#define GAME_BTN_SRC_W (60)
#define GAME_BTN_W X(GAME_BTN_SRC_W)
#define GAME_BTN_Y X(408)
#define GAME_BTN_LEFT_X X(77)
#define GAME_BTN_RIGHT_X X(294)
#define GAME_BTN_CENTER_X X(186)
#define GAME_BTN_DUB_X X(186)
#define GAME_BTN_DUB_Y X(38)
#define GAME_BTN_SRC_OFFSET_DUB (0)
#define GAME_BTN_SRC_OFFSET_CONF (GAME_BTN_SRC_W)
#define GAME_BTN_SRC_OFFSET_ROLL (GAME_BTN_SRC_W * 2)
#define GAME_BTN_SRC_OFFSET_UNDO (GAME_BTN_SRC_W * 3)
#define GAME_BTN_SRC_OFFSET_NOMOVES (GAME_BTN_SRC_W * 4)

#define GAME_BTN_SRC_OFFSET GAME_BTN_SRC_W

#define OVERLAY_SHADE_SRC_W X(430)
#define OVERLAY_SHADE_SRC_H X(932)
#define OVERLAY_SHADE_W OVERLAY_SHADE_SRC_W
#define OVERLAY_SHADE_H OVERLAY_SHADE_SRC_H
#define OVERLAY_SHADE_X X(0)
#define OVERLAY_SHADE_Y X(0)

#define DIALOG_SRC_W X(420)
#define DIALOG_SRC_H X(250)
#define DIALOG_W DIALOG_SRC_W
#define DIALOG_H DIALOG_SRC_H
#define DIALOG_X X(6)
#define DIALOG_Y X(318)
#define DIALOG_START_Y OVERLAY_SHADE_H

#define DIALOG_BTN_SRC_W X(80)
#define DIALOG_BTN_SRC_H X(35)
#define DIALOG_BTN_W DIALOG_BTN_SRC_W
#define DIALOG_BTN_H DIALOG_BTN_SRC_H
#define DIALOG_BTN_LEFT_X X(125)  // relative to dialog box
#define DIALOG_BTN_RIGHT_X X(215) // relative to dialog box
#define DIALOG_BTN_Y X(168)       // relative to dialog box

#define DIALOG_TITLE_X (DIALOG_W / 2)
#define DIALOG_TITLE_Y X(30)
#define DIALOG_MATCHTO_X (DIALOG_W / 2)
#define DIALOG_MATCHTO_Y X(70)
#define DIALOG_WON_X (DIALOG_W / 2)
#define DIALOG_WON_Y X(90)
#define DIALOG_LOST_X (DIALOG_W / 2)
#define DIALOG_LOST_Y X(110)

#define CHECKER_VELOCITY X(20)
#define DICE_VELOCITY X(5)

#define SCORE_LEFT_X X(65)
#define SCORE_RIGHT_X X(331)
#define SCORE_Y X(43)
#define MATCH_SCORE_Y X(67)

#define SCORE_FONT_SRC_SIZE (20)
#define SCORE_FONT_SIZE X(SCORE_FONT_SRC_SIZE)

#define DEBUG_FONT_SRC_SIZE (20)
#define DEBUG_FONT_SIZE X(20)
#define DEBUG_X X(2)
#define DEBUG_Y BOARD_Y + BOARD_H

#define EPSILON 0.0001

bool isBetween(Sint32 x, Sint32 min, Sint32 max);
bool isEqual(float x, float y, float epsilon);
int generateRandomNumber(int min, int max);
