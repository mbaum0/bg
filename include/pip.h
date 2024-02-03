/**
 * @file pip.h
 * @author Michael Baumgarten
 * @brief pip objects
 */
#pragma once
#include <stdint.h>

#define CAP_PIP_INDEX(x) ((x) < 0 ? 0 : ((x) > 25 ? 25 : (x)))
#define PLAYER_HOME(p) ((p == LIGHT) ? LIGHT_HOME : DARK_HOME)

void createPipSprites(void);
