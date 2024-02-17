/**
 * @file util.c
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
bool isBetween(Sint32 x, Sint32 min, Sint32 max) {
    return x >= min && x <= max;
}

bool isEqual(float x, float y, float epsilon) {
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
