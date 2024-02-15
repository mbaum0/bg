/**
 * @file vector.c
 * @author Michael Baumgarten
 * @brief Vector math implementations
 */

#include "vector.h"
#include "sage.h"

float getVerticalVelocity(float velocity, float x0, float y0, float x1, float y1) {
    float hypotenuse = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
    float cosOrigin = fabs(y1 - y0) / hypotenuse;
    float yVelocity = cosOrigin * velocity;
    return yVelocity *= (y0 < y1) ? 1 : -1;
}

float getHorizontalVelocity(float velocity, float x0, float y0, float x1, float y1) {
    float hypotenuse = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
    float sinOrigin = fabs(x1 - x0) / hypotenuse;
    float xVelocity = sinOrigin * velocity;
    return xVelocity *= (x0 < x1) ? 1 : -1;
}

void getNextCoordinatesCircle(float radius, float velocity, float h, float k, float x, float y, float* nextX,
                              float* nextY) {
    // Calculate the current angle between the current position and the center of the circle
    float angle = atan2(y - k, x - h);

    // calculate the next angle by adding the angular velocity
    float angularVelocity = velocity / radius;
    angle += angularVelocity;

    *nextX = h + radius * cos(angle);
    *nextY = k + radius * sin(angle);
}

float getVelocityFromAcceleration(float acceleration, float velocity, float x0, float x1, float y0, float y1, float* h,
                                  float* v) {
    (void)acceleration;
    (void)velocity;
    (void)x0;
    (void)x1;
    (void)y0;
    (void)y1;
    (void)h;
    (void)v;
    return 0.0;
}
