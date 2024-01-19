/**
 * @file util.c
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#include "util.h"
bool isBetween(int32_t x, int32_t min, int32_t max)
{
  return x >= min && x <= max;
}
