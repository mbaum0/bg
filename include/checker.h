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
typedef enum { LIGHT, DARK, NONE, BLOT } Color;

#define OPPONENT_COLOR(c) (c == LIGHT ? DARK : LIGHT)

struct Checker {
  int32_t pipIndex;
  int32_t pipOffset;
  Color color;
};

void createCheckerSprite(Checker* c);
