/**
 * @file input.h
 * @author Michael Baumgarten
 * @brief 
 */
#pragma once

#include <SDL.h>
#include <stdbool.h>
#include "log.h"

/**
 * @brief Processes input from SDL
 * 
 * @return true if the game should quit
 */
bool processInput(void);
