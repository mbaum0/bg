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

void findDimensions(int w, int h, int* x, int* y) {
  // Start from the maximum width which is 90% of w
  for (int possibleX = (int)(MAX_BOARD_WIDTH_PCT * w); possibleX > 0; possibleX--) {
    // For each width, find a height that maintains the aspect ratio
    for (int possibleY = h; possibleY > 0; possibleY--) {
      float aspectRatio = (float)possibleX / possibleY;
      // Check if the aspect ratio is within the range
      if (aspectRatio >= MIN_ASPECT_RATIO && aspectRatio <= MAX_ASPECT_RATIO) {
        *x = possibleX;
        *y = possibleY;
        return;
      }
    }
  }

  // If no suitable dimensions are found, return 0
  *x = 0;
  *y = 0;
}
