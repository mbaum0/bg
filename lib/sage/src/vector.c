/**
 * @file vector.c
 * @author Michael Baumgarten
 * @brief Vector math implementations
 */

#include "vector.h"

int32_t getVerticalVelocity(int32_t velocity, int32_t x0, int32_t y0, int32_t x1, int32_t y1){
    float hypotenuse = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
    float cosOrigin = abs(y1 - y0) / hypotenuse;
    int32_t yVelocity = cosOrigin * velocity;
    return yVelocity *= (y0 < y1) ? 1 : -1;
}

int32_t getHorizontalVelocity(int32_t velocity, int32_t x0, int32_t y0, int32_t x1, int32_t y1){
    float hypotenuse = sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
    float sinOrigin = abs(x1 - x0) / hypotenuse;
    int32_t xVelocity = sinOrigin * velocity;
    return xVelocity *= (x0 < x1) ? 1 : -1; 
}
