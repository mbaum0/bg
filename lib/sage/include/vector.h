/**
 * @file vector.h
 * @author Michael Baumgarten
 * @brief Vector math implementations
 */
#pragma once
#include <math.h>
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
float getVerticalVelocity(float velocity, float x0, float y0, float x1, float y1);

/**
 * @brief Returns the horizontal component of a velocity vector given the start and end points
 *
 * @param velocity The magnitude of the velocity vector
 * @param x0 Starting x coordinate
 * @param y0 Starting y coordinate
 * @param x1 Ending x coordinate
 * @param y1 Ending y coordinate
 */
float getHorizontalVelocity(float velocity, float x0, float y0, float x1, float y1);

float getVelocityFromAcceleration(float acceleration, float velocity, float x0, float x1, float y0, float y1, float* h, float* v);

void getNextCoordinatesCircle(float radius, float velocity, float h, float k, float x, float y, float* nextX, float* nextY);
