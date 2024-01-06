/**
 * @file checker.h
 * @author Michael Baumgarten
 * @brief
 */
#pragma once
#include "sage.h"
#include <stdint.h>

#define CHECKER_SIZE 80
typedef struct Checker Checker;

struct Checker {
  int32_t index;
};

Checker* Checker_create(Sage* sage, int32_t index);

void Checker_destroy(Checker* c);
