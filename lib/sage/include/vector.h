/**
 * @file vector.h
 * @author Michael Baumgarten
 * @brief Vector math implementations
 */
#pragma once
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

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
