/**
 * @file input.h
 * @author Michael Baumgarten
 * @brief
 */
#pragma once

#include "log.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

/**
 * @brief Processes input from SDL
 *
 * @return true if the game should quit
 */
bool processInput(void);
