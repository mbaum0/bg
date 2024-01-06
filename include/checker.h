/**
 * @file checker.h
 * @author Michael Baumgarten
 * @brief 
 */
#pragma once
#include <stdint.h>
#include "sage.h"

#define CHECKER_SIZE 80
typedef struct Checker Checker;


struct Checker {
    int32_t index;
};

void Checker_create(Sage* sage, int32_t index);
