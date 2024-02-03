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

#define LIGHT_HOME_START 19
#define LIGHT_HOME_END 24
#define LIGHT_HOME 25
#define DARK_HOME_START 0
#define DARK_HOME_END 6
#define DARK_HOME 0
#define LIGHT_BAR 26
#define DARK_BAR 27

#define OPPONENT_COLOR(c) (c == LIGHT ? DARK : LIGHT)

struct Checker {
  int32_t pipIndex;
  int32_t pipOffset;
  Color color;
};

void createCheckerSprite(Checker* c);
