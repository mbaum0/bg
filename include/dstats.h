/**
 * @file dstats.h
 * @author Michael Baumgarten
 * @brief Debug stats for the game
 */
#pragma once
#include "view.h"
#include "media.h"
#include "board.h"

void DStats_createSnippet(MediaManager* mm, ViewManager* vm, GameBoard* board);
