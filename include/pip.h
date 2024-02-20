/**
 * @file pip.h
 * @author Michael Baumgarten
 * @brief pip objects
 */
#pragma once
#include <SDL3/SDL.h>

#define CAP_PIP_INDEX(x) ((x) < 0 ? 0 : ((x) > 25 ? 25 : (x)))
#define PLAYER_HOME(p) ((p == LIGHT) ? LIGHT_HOME : DARK_HOME)

typedef struct Pip Pip;

struct Pip {
    Uint32 alpha; // transparency of the pip
    Uint32 index; // index of this pip
};

void createPipSprite(Pip* pip);
