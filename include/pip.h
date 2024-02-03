/**
 * @file pip.h
 * @author Michael Baumgarten
 * @brief pip objects
 */
#pragma once
#include <stdint.h>

#define DARK_HOME_START 19
#define LIGHT_HOME_START 6
#define DARK_HOME 25
#define LIGHT_HOME 0
#define CAP_PIP_INDEX(x) ((x) < 0 ? 0 : ((x) > 25 ? 25 : (x)))
#define PLAYER_HOME(p) ((p == LIGHT) ? LIGHT_HOME : DARK_HOME)

void createPipSprites(void);
