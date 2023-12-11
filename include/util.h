/**
 * @file util.h
 * @author Michael Baumgarten
 * @brief Things that don't fit anywhere else
 */
#pragma once
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    P_Dark,
    P_Light,
    P_None
} Player;

typedef enum {
    B_Confirm,
    B_Undo,
    B_Roll,
} ButtonType;

/**
 * @brief Returns the vertical component of a velocity vector given the start and end points
 * 
 * @param velocity The magnitude of the velocity vector
 * @param x0 Starting x coordinate
 * @param y0 Starting y coordinate
 * @param x1 Ending x coordinate
 * @param y1 Ending y coordinate
 */
int32_t getVerticalVelocity(int32_t velocity, int32_t x0, int32_t y0, int32_t x1, int32_t y1);

/**
 * @brief Returns the horizontal component of a velocity vector given the start and end points
 * 
 * @param velocity The magnitude of the velocity vector
 * @param x0 Starting x coordinate
 * @param y0 Starting y coordinate
 * @param x1 Ending x coordinate
 * @param y1 Ending y coordinate
 */
int32_t getHorizontalVelocity(int32_t velocity, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
