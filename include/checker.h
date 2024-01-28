/**
 * @file checker.h
 * @author Michael Baumgarten
 * @brief
 */
#pragma once
#include <stdint.h>
#include "sage.h"
#include "board.h"

typedef struct Checker Checker;
typedef enum { LIGHT, DARK} Color;

struct Checker {
  int32_t pipIndex;
  int32_t pipYOffset;
  int32_t pipXOffset;
  Color color;
};

void createCheckerSprite(Checker* c);
