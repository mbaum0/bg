/**
 * @file util.c
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#include "util.h"
#include <math.h>
bool isBetween(int32_t x, int32_t min, int32_t max)
{
  return x >= min && x <= max;
}

bool isEqual(float x, float y, float epsilon){
  return fabs(x - y) < epsilon;
}
