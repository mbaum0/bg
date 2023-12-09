/**
 * @file pip.h
 * @author Michael Baumgarten
 * @brief Contains pip related functionality
 */
#pragma once
#include <stdint.h>
#include "events.h"
#include "media.h"
#include "view.h"

/**
 * @brief Create a sprite for a given pip
 */
void Pip_createSprite(uint32_t pip, MediaManager* mm, ViewManager* vm);
