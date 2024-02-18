/**
 * @file checker.h
 * @author Michael Baumgarten
 * @brief
 */
#pragma once
#include "board.h"
#include "fsm.h"
#include "sage.h"

typedef struct Checker Checker;
typedef enum { LIGHT, DARK, NONE } Color;

#define LIGHT_HOME_START 19
#define LIGHT_HOME_END 25
#define LIGHT_HOME 25
#define DARK_HOME_START 0
#define DARK_HOME_END 6
#define DARK_HOME 0
#define LIGHT_BAR 26
#define DARK_BAR 27

#define OPPONENT_COLOR(c) (c == LIGHT ? DARK : LIGHT)
#define CHECKER_IS_BARRED(c)                                                                                           \
    ((c->color == DARK && c->pipIndex == DARK_BAR) || (c->color == LIGHT && c->pipIndex == LIGHT_BAR))

struct Checker {
    Sint32 pipIndex;  // pip the checker is on
    Sint32 pipOffset; // position of this checker on the pip
    Color color;
};

void createCheckerSprite(Checker* c);
