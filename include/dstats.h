/**
 * @file dstats.h
 * @author Michael Baumgarten
 * @brief Debug stats for the game
 */
#pragma once
#include "media.h"
#include "board.h"
#include "vmanager.h"
#include "snippet.h"

void DStats_createSnippet(MediaManager* mm, ViewManager* vm, GameBoard* board);
