/**
 * @file util.h
 * @author Michael Baumgarten
 * @brief Utility functions
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define Z_BACKGROUND 0 
#define Z_BOARD 1
#define Z_PIPS 2
#define Z_TRIM 3

bool isBetween(int32_t x, int32_t min, int32_t max);
