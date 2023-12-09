/**
 * @file checker.h
 * @author Michael Baumgarten
 * @brief Contains checker related functionality
 */
#pragma once
#include <stdint.h>
#include "util.h"
#include "view.h"
#include "events.h"
#include "media.h"
#include <math.h>

typedef struct Checker Checker;

struct Checker {
    Player player; // player that owns the checker
    int32_t location; // 0 - light's home, 1 - 24 - pips, 25 - dark's home
    int32_t index; // the index of the checker at a given location (between 0 and 4 if on a pip)
};

/**
 * @brief Create a sprite for a given checker
 */
void Checker_createSprite(Checker* checker, MediaManager* mm, ViewManager* vm);
