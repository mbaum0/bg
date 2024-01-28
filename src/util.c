/**
 * @file util.c
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#include "util.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
bool isBetween(int32_t x, int32_t min, int32_t max)
{
  return x >= min && x <= max;
}

bool isEqual(float x, float y, float epsilon){
  return fabs(x - y) < epsilon;
}


int generateRandomNumber(int min, int max) {
    // Seed the random number generator with the current time
    
    // Generate a random number in the range [0, RAND_MAX]
    int random_num = rand();
    
    // Scale and shift the random number to the desired range [min, max]
    int range = max - min + 1;
    int scaled_random_num = (random_num % range) + min;
    
    return scaled_random_num;
}
